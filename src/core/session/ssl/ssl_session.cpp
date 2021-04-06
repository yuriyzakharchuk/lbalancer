#include "ssl_session.hpp"

#include <iostream>

using namespace lb::session;

ssl_session::ssl_session(boost::asio::executor &&executor,
                         ssl_context_t &frontend_ssl_context,
                         ssl_context_t &backend_ssl_context,
                         socket_t &&frontend_socket, backend_t &backend)
    : backend_(backend),
      resolver_(executor),
      frontend_stream_(std::move(frontend_socket), frontend_ssl_context),
      backend_stream_(executor, backend_ssl_context) {}

void ssl_session::start_session() {
    boost::certify::set_server_hostname(backend_stream_, backend_.server());
    boost::certify::sni_hostname(backend_stream_, backend_.server());

    if (!SSL_set_tlsext_host_name(backend_stream_.native_handle(),
                                  backend_.server().c_str())) {
        // TODO: handle error
        // beast::error_code error { static_cast<int>(::ERR_get_error()),
        // boost::asio::error::get_ssl_category()};
        // << error.message() << "\n";
        return;
    }
    resolver_.async_resolve(
        backend_.server(), backend_.port(),
        beast::bind_front_handler(&ssl_session::backend_on_resolve,
                                  shared_from_this()));
}

void ssl_session::frontend_read() {
    frontend_request_ = {};
    http::async_read(frontend_stream_, frontend_buffer_, frontend_request_,
                     beast::bind_front_handler(&ssl_session::frontend_on_read,
                                               shared_from_this()));
}

void ssl_session::frontend_write() {
    http::async_write(frontend_stream_, frontend_response_,
                      beast::bind_front_handler(&ssl_session::frontend_on_write,
                                                shared_from_this()));
}

void ssl_session::frontend_on_read(beast::error_code error,
                                   std::size_t bytes_transferred) {
    boost::ignore_unused(bytes_transferred);
    if (error == http::error::end_of_stream) {
        return frontend_close();
    }
    if (error) {
        return;
    }
    handle_frontend_request(std::move(frontend_request_));
}

void ssl_session::frontend_on_write(beast::error_code ec,
                                    std::size_t bytes_transferred) {
    boost::ignore_unused(bytes_transferred);
    if (ec) {
        return;
    }
    frontend_read();
}

void ssl_session::handle_frontend_request(
    http::request<http::string_body> &&request) {
    // TODO: handle request and set up backend_request_
    backend_request_ = request;
    backend_request_.set(http::field::host, BOOST_BEAST_VERSION_STRING);
    backend_write();
}

void ssl_session::frontend_close() {
    beast::error_code error;
    frontend_stream_.lowest_layer().shutdown(socket_t ::shutdown_send, error);
    if (error) {
        // TODO: handle error
    }
}

void ssl_session::backend_on_resolve(beast::error_code error,
                                     const resolver_t::results_type &results) {
    if (error) {
        return;
    }
    backend_stream_.lowest_layer().async_connect(
        results->endpoint(),
        beast::bind_front_handler(&ssl_session::backend_on_connect,
                                  shared_from_this()));
}

void ssl_session::backend_on_connect(beast::error_code error) {
    if (error) {
        // TODO: handle error
        return;
    }
    backend_stream_.async_handshake(
        boost::asio::ssl::stream_base::client,
        [c = shared_from_this()](beast::error_code error) {
            if (error) {
                std::cout << error.message() << std::endl;
                return;
            }
            c->frontend_stream_.async_handshake(
                boost::asio::ssl::stream_base::server,
                [ch = c->shared_from_this()](
                    const boost::system::error_code &error) {
                    if (error) {
                        std::cout << error.message() << std::endl;
                        // TODO: handle error
                        return;
                    }
                    ch->frontend_read();
                });
        });
}

void ssl_session::backend_read() {
    backend_response_ = {};
    http::async_read(backend_stream_, backend_buffer_, backend_response_,
                     beast::bind_front_handler(&ssl_session::backend_on_read,
                                               shared_from_this()));
}

void ssl_session::backend_write() {
    http::async_write(backend_stream_, backend_request_,
                      beast::bind_front_handler(&ssl_session::backend_on_write,
                                                shared_from_this()));
}

void ssl_session::backend_on_read(beast::error_code error,
                                  std::size_t bytes_transferred) {
    boost::ignore_unused(bytes_transferred);
    if (error == http::error::end_of_stream) {
        return backend_close();
    }
    if (error) {
        return;
    }
    handle_backend_response(std::move(backend_response_));
}

void ssl_session::backend_on_write(beast::error_code error,
                                   std::size_t bytes_transferred) {
    boost::ignore_unused(bytes_transferred);
    if (error) {
        std::cout << error.message() << std::endl;
        return;
    }
    backend_read();
}

void ssl_session::handle_backend_response(
    http::response<http::string_body> &&response) {
    // TODO: handle response and set up frontend_response_
    frontend_response_ = response;
    frontend_response_.set(http::field::host, "boost_asio.com");
    frontend_write();
}

void ssl_session::backend_close() {
    beast::error_code error;
    backend_stream_.lowest_layer().shutdown(socket_t ::shutdown_send, error);
    if (error) {
    }
}
