#include "tcp_session.hpp"


lb::session::tcp_session::tcp_session(boost::asio::io_service &service, backend_t &backend, std::size_t buffer_len)
    : lb::session::basic_session(service, backend),
      buffer_len_(buffer_len),
      fb_buffer_(),
      bf_buffer_() {
}


void
lb::session::tcp_session::start_session() {
    frontend_socket_.async_receive(fb_buffer_.prepare(buffer_len_),
                                   0,
                                   [c = shared_from_this()](const boost::system::error_code& error,
                                                            std::size_t bytes_transferred) {
        if(!error) {
            if(bytes_transferred > 0) {
               c->fb_buffer_.commit(bytes_transferred);
               resolver_t resolver(c->service_);
               auto endpoints {
                       resolver.resolve(c->backend_.server(), c->backend_.port())
               };
               boost::asio::async_connect(c->backend_socket_, endpoints,
                                          [cn = c->shared_from_this()](auto &&error, auto &&endpoint) {
                                              cn->backend_write();
                                          });
            }
        }
        else {
           c->frontend_socket_.close();
        }
    });
}


void
lb::session::tcp_session::frontend_read() {
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


void
lb::session::tcp_session::frontend_write() {
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


void
lb::session::tcp_session::backend_read() {
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


void
lb::session::tcp_session::backend_write() {
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

