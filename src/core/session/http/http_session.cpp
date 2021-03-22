#include <iostream>

#include "http_session.hpp"


using namespace lb::session;


http_session::http_session(boost::asio::io_service &service,
                           socket_t&& frontend_socket,
                           backend_t &backend)
    : service_(service),
      backend_(backend),
      frontend_stream_(std::move(frontend_socket)),
      backend_stream_(service) {
}


void http_session::start_session() {
    resolver_t resolver(service_);
    auto endpoints {
            resolver.resolve(backend_.server(), backend_.port())
    };
    backend_stream_.async_connect(endpoints, [chh = shared_from_this()]
            (beast::error_code ec, const http_session::resolver_t::results_type::endpoint_type&) {
        if(ec) {
            auto message = ec.message();
            std::cout << message << std::endl;
            return;
        }
        chh->frontend_read();
    });
}


void http_session::frontend_read() {
    frontend_request_ = {};
    //frontend_stream_.expires_after(std::chrono::seconds(30));
    http::async_read(frontend_stream_, frontend_buffer_, frontend_request_,
                     beast::bind_front_handler(
                             &http_session::frontend_on_read,
                             shared_from_this()));
}


void http_session::frontend_write() {
    http::async_write(frontend_stream_,
                      frontend_response_,
                      beast::bind_front_handler(
                              &http_session::frontend_on_write,
                              shared_from_this()));
}


void http_session::frontend_on_read(beast::error_code ec,
                               std::size_t bytes_transferred) {
    boost::ignore_unused(bytes_transferred);
    if(ec == http::error::end_of_stream) {
        return frontend_close();
    }
    if(ec) {
        auto message = ec.message();
        std::cout << message << std::endl;
        return;
        //return fail(ec, "read");
    }
    handle_frontend_request(std::move(frontend_request_));
}


void http_session::frontend_on_write(beast::error_code ec,
                               std::size_t bytes_transferred) {
    boost::ignore_unused(bytes_transferred);
    if(ec) {
        // return fail(ec, "write");
    }
    frontend_read();
}


void
http_session::handle_frontend_request(http::request<http::string_body> &&request) {
    // TODO: handle request and set up backend_request_
    backend_request_ = {};
    backend_write();
}


void http_session::frontend_close() {
    // Send a TCP shutdown
    beast::error_code ec;
    frontend_stream_.socket().shutdown(socket_t ::shutdown_send, ec);
    // At this point the connection is closed gracefully
}


void http_session::backend_read() {
    backend_response_ = {};
    http::async_read(backend_stream_, backend_buffer_, backend_response_,
                     beast::bind_front_handler(
                             &http_session::backend_on_read,
                             shared_from_this()));
}


void http_session::backend_write() {
    http::async_write(backend_stream_,
                      backend_request_,
                      beast::bind_front_handler(
                              &http_session::backend_on_write,
                              shared_from_this()));
}


void http_session::backend_on_read(beast::error_code ec,
                               std::size_t bytes_transferred) {
    boost::ignore_unused(bytes_transferred);
    // This means they closed the connection
    if(ec == http::error::end_of_stream) {
        return backend_close();
    }
    if(ec) {
        return;
        //return fail(ec, "read");
    }
    handle_backend_response(std::move(backend_response_));
}


void http_session::backend_on_write(beast::error_code ec,
                                std::size_t bytes_transferred) {
    boost::ignore_unused(bytes_transferred);
    if(ec) {
        // return fail(ec, "write");
    }
    backend_read();
}


void http_session::backend_close() {
    beast::error_code ec;
    backend_stream_.socket().shutdown(socket_t ::shutdown_send, ec);
    // At this point the connection is closed gracefully
    // TODO: handle error
}


void
http_session::handle_backend_response(http::response<http::string_body> &&response) {
    // TODO: handle response and set up frontend_response_
    frontend_response_ = {};
    frontend_write();
}

