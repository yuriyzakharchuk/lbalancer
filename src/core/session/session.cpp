#include "session.hpp"

#include <iostream>
#include <string> // TODO: debug


lb::session::session::session(boost::asio::io_service &service,
                              backend_t &backend,
                              session_handler &)
        : service_(service), backend_(backend), frontend_socket_(service),
          backend_socket_(service), fb_buffer_(), bf_buffer_() {
}


lb::session::session::socket_t &
lb::session::session::get_socket() {
    return frontend_socket_;
}


void
lb::session::session::start_session() {
    frontend_socket_.async_receive(fb_buffer_.prepare(8192),
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
           //std::cout << "start_session() " << error.message() << std::endl;
           c->frontend_socket_.close();
        }
    });
}


void
lb::session::session::frontend_read() {
    frontend_socket_.async_receive(fb_buffer_.prepare(8192),
                                   0,
                                   [c = shared_from_this()](const boost::system::error_code& error,
                                                  std::size_t bytes_transferred) {
        if(!error) {
            //std::cout << "fb size: " << c->fb_buffer_.size() << std::endl;
            if(bytes_transferred > 0) {
                c->fb_buffer_.commit(bytes_transferred);
                c->backend_write();
                c->frontend_read();
            }
        }
        else {
            //std::cout << "frontend_read() " << error.message() << std::endl;
            c->frontend_socket_.close();
        }
    });
}


void
lb::session::session::frontend_write() {
    frontend_socket_.async_send(bf_buffer_.data(),
                               [c = shared_from_this()](const boost::system::error_code& error,
                                                        std::size_t bytes_transferred) {
       if(!error) {
           //std::cout << "bf size: " << c->bf_buffer_.size() << std::endl;
           if(bytes_transferred > 0) {
               c->bf_buffer_.consume(bytes_transferred);
               c->frontend_read();
               c->frontend_write();
           }
       }
       else {
           //std::cout << "frontend_write() " << error.message() << std::endl;
           c->frontend_socket_.close();
       }
   });
}


void
lb::session::session::backend_read() {
    backend_socket_.async_receive(bf_buffer_.prepare(8192),
                                   0,
                                   [c = shared_from_this()](const boost::system::error_code& error,
                                                            std::size_t bytes_transferred) {
        if(!error) {
            //std::cout << "bf size: " << c->bf_buffer_.size() << std::endl;
            if(bytes_transferred > 0) {
                c->bf_buffer_.commit(bytes_transferred);
                c->frontend_write();
                c->backend_read();
            }
        }
        else {
           //std::cout << "backend_read() " << error.message() << std::endl;
           c->backend_socket_.close();
        }
    });
}


void
lb::session::session::backend_write() {
    backend_socket_.async_send(fb_buffer_.data(),
                               [c = shared_from_this()](const boost::system::error_code& error,
                                       std::size_t bytes_transferred) {
        if(!error) {
            //std::cout << "fb size: " << c->fb_buffer_.size() << std::endl;
            c->fb_buffer_.consume(bytes_transferred);
            if(c->fb_buffer_.size() > 0) {
                c->backend_write();
            }
            c->backend_read();
        }
        else  {
            //std::cout << "backend_write() " << error.message() << std::endl;
            c->backend_socket_.close();
        }
    });
}

