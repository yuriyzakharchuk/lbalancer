#include "session.hpp"

#include <iostream>
#include <string> // TODO: debug


lb::session::session::session(boost::asio::io_service &service,
                              backend_t &backend,
                              session_handler &)
        : service_(service), backend_(backend), frontend_socket_(service),
          backend_socket_(service), frontend_reading_buffer_(),
          backend_reading_buffer_() {

}


lb::session::session::socket_t &
lb::session::session::get_socket() {
    return frontend_socket_;
}


void
lb::session::session::run() {
    resolver_t resolver(service_);
    auto endpoints {
            resolver.resolve(backend_.server(), backend_.port())
    };
    boost::asio::async_connect(backend_socket_, endpoints,
                               [c = shared_from_this()](auto &&error,
                                                        auto &&endpoint) {
                                   c->start_session(error, endpoint);
                               });
}


void
lb::session::session::handle_frontend_reading(const boost::system::error_code &error,
                                              std::size_t bytes_transferred) {
    frontend_reading_buffer_.commit(bytes_transferred);
    if (!error) {
        boost::asio::async_write(backend_socket_, frontend_reading_buffer_,
                                 [c = shared_from_this()](auto &&error,
                                                          auto &&bytes_transferred) {
                                     std::cout << "to backend: "
                                               << bytes_transferred
                                               << std::endl;
                                     c->handle_backend_writing(error, bytes_transferred);
                                 });

        boost::asio::async_read(frontend_socket_, frontend_reading_buffer_,
                                boost::asio::transfer_at_least(1),
                                [c = shared_from_this()](auto &&error,
                                                         auto &&bytes_transferred) {
                                    std::cout << "from client: "
                                              << bytes_transferred << std::endl;
                                    c->handle_frontend_reading(error, bytes_transferred);
                                });
    }
    else {
        std::cout << "Error: handle_frontend_reading - " << error.message()
                  << std::endl;
        //TODO: handling error
    }
}


void
lb::session::session::handle_frontend_writing(const boost::system::error_code &error,
                                              std::size_t bytes_transferred) {
    backend_reading_buffer_.consume(bytes_transferred);
    if (!error) {
        boost::asio::async_read(frontend_socket_, frontend_reading_buffer_,
                                boost::asio::transfer_at_least(1),
                                [c = shared_from_this()](auto &&error,
                                                         auto &&bytes_transferred) {
                                    std::cout << "to client: "
                                              << bytes_transferred << std::endl;
                                    c->handle_frontend_reading(error, bytes_transferred);
                                });
        boost::asio::async_read(backend_socket_, backend_reading_buffer_,
                                boost::asio::transfer_at_least(1),
                                [c = shared_from_this()](auto &&error,
                                                         auto &&bytes_transferred) {
                                    //TODO: check if socket available
                                    std::cout << "from backend: "
                                              << bytes_transferred << std::endl;
                                    c->handle_backend_reading(error, bytes_transferred);
                                });
    }
    else {
        std::cout << "Error: handle_frontend_writing - " << error.message()
                  << std::endl;
        //TODO: handling error
    }
}


void
lb::session::session::handle_backend_reading(const boost::system::error_code &error,
                                             std::size_t bytes_transferred) {
    backend_reading_buffer_.commit(bytes_transferred);
    if (!error) {
        boost::asio::async_read(backend_socket_, backend_reading_buffer_,
                                boost::asio::transfer_at_least(1),
                                [c = shared_from_this()](auto &&error,
                                                         auto &&bytes_transferred) {
                                    //TODO: check if socket available
                                    std::cout << "from backend: "
                                              << bytes_transferred << std::endl;
                                    c->handle_backend_reading(error, bytes_transferred);
                                });
    }
    else {
        std::cout << "Error: handle_backend_reading - " << error.message()
                  << std::endl;
        //TODO: handling error
        boost::asio::async_write(frontend_socket_, backend_reading_buffer_,
                                 [c = shared_from_this()](auto &&error,
                                                          auto &&bytes_transferred) {
                                     std::cout << "to client: "
                                               << bytes_transferred
                                               << std::endl;
                                     c->handle_frontend_writing(error, bytes_transferred);
                                 });
        frontend_socket_.close();
    }
}


void
lb::session::session::handle_backend_writing(const boost::system::error_code &error,
                                             std::size_t bytes_transferred) {
    frontend_reading_buffer_.consume(bytes_transferred);
    if (!error) {
        boost::asio::async_read(backend_socket_, backend_reading_buffer_,
                                boost::asio::transfer_at_least(1),
                                [c = shared_from_this()](auto &&error,
                                                         auto &&bytes_transferred) {
                                    //TODO: check if socket available
                                    std::cout << "from backend: "
                                              << bytes_transferred << std::endl;
                                    c->handle_backend_reading(error, bytes_transferred);
                                });
    }
    else {
        //TODO: handling error
        std::cout << "Error: handle_backend_writing - " << error.message()
                  << std::endl;
    }
}


void
lb::session::session::start_session(const boost::system::error_code &error,
                                    const boost::asio::ip::tcp::endpoint &endpoint) {
    if (!error) {
        boost::asio::async_read(frontend_socket_, frontend_reading_buffer_,
                                boost::asio::transfer_at_least(1),
                                [c = shared_from_this()](auto &&error,
                                                         auto &&bytes_transferred) {
                                    std::cout << "from client: "
                                              << bytes_transferred
                                              << std::endl;
                                    c->handle_frontend_reading(error, bytes_transferred);
                                });
    }
    else {
        std::cout << "Error: start_session - " << error.message()
                  << std::endl;
    }
}

