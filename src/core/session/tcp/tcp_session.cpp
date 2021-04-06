#include "tcp_session.hpp"

using namespace lb::session;


tcp_session::tcp_session(socket_t&& frontend_socket,
                         backend_t& backend,
                         std::size_t buffer_len)
    : frontend_socket_(std::move(frontend_socket)),
      backend_socket_(frontend_socket_.get_executor()),
      backend_(backend),
      fb_buffer_(),
      bf_buffer_(),
      buffer_len_(buffer_len) {
}


void tcp_session::start_session() {
    frontend_socket_.async_receive(fb_buffer_.prepare(buffer_len_),
                                   0,
                                   [c = shared_from_this()](const boost::system::error_code& error,
                                                            std::size_t bytes_transferred) {
        if(!error) {
            if(bytes_transferred > 0) {
               c->fb_buffer_.commit(bytes_transferred);
               resolver_t resolver(c->backend_socket_.get_executor());
               auto endpoints {
                   resolver.resolve(c->backend_.server(), c->backend_.port())
               };
               boost::asio::async_connect(c->backend_socket_, endpoints,
                                          [cn = c->shared_from_this()](auto &&error, auto &&) {
                                              if(error) {
                                                 return;
                                              }
                                              cn->backend_write();
                                          });
            }
        }
        else {
           c->frontend_socket_.close();
        }
    });
}


void tcp_session::frontend_read() {
    frontend_socket_.async_receive(fb_buffer_.prepare(buffer_len_),
                                   0,
                                   [c = shared_from_this()](const boost::system::error_code& error,
                                                  std::size_t bytes_transferred) {
        if(!error) {
            if(bytes_transferred > 0) {
                c->fb_buffer_.commit(bytes_transferred);
                c->backend_write();
                c->frontend_read();
            }
        }
        else {
            c->frontend_socket_.close();
        }
    });
}


void tcp_session::frontend_write() {
    frontend_socket_.async_send(bf_buffer_.data(),
                               [c = shared_from_this()](const boost::system::error_code& error,
                                                        std::size_t bytes_transferred) {
       if(!error) {
           if(bytes_transferred > 0) {
               c->bf_buffer_.consume(bytes_transferred);
               c->frontend_read();
           }
       }
       else {
           c->frontend_socket_.close();
       }
   });
}


void tcp_session::backend_read() {
    backend_socket_.async_receive(bf_buffer_.prepare(buffer_len_),
                                   0,
                                   [c = shared_from_this()](const boost::system::error_code& error,
                                                            std::size_t bytes_transferred) {
        if(!error) {
            if(bytes_transferred > 0) {
                c->bf_buffer_.commit(bytes_transferred);
                c->frontend_write();
                c->backend_read();
            }
        }
        else {
           c->backend_socket_.close();
        }
    });
}


void tcp_session::backend_write() {
    backend_socket_.async_send(fb_buffer_.data(),
                               [c = shared_from_this()](const boost::system::error_code& error,
                                       std::size_t bytes_transferred) {
        if(!error) {
            c->fb_buffer_.consume(bytes_transferred);
            if(c->fb_buffer_.size() > 0) {
                c->backend_write();
            }
            c->backend_read();
        }
        else  {
            c->backend_socket_.close();
        }
    });
}

