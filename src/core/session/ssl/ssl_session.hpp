#ifndef LOAD_BALANCER_SSL_SESSION_HPP
#define LOAD_BALANCER_SSL_SESSION_HPP

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/beast/version.hpp>
#include <boost/certify/extensions.hpp>
#include <boost/certify/https_verification.hpp>

#include "../../backend/backend.hpp"

namespace beast = boost::beast;
namespace http = beast::http;

namespace lb::session {
class ssl_session : public std::enable_shared_from_this<ssl_session> {
   public:
    using ssl_context_t = boost::asio::ssl::context;
    using resolver_t = boost::asio::ip::tcp::resolver;
    using backend_t = backend::backend;
    using socket_t = boost::asio::ip::tcp::socket;

    ssl_session(boost::asio::executor &&executor,
                ssl_context_t &frontend_ssl_context,
                ssl_context_t &backend_ssl_context, socket_t &&frontend_socket,
                backend_t &backend);

    static inline void load_server_certificate(
        ssl_context_t &context, const std::string &certificate_path,
        const std::string &key_path) {
        // TODO: get certs path values from configurator
        context.use_certificate_chain_file(certificate_path);
        context.use_private_key_file(key_path, ssl_context_t::file_format::pem);
    }

    static inline void verify_certificate(ssl_context_t &context) {
        context.set_verify_mode(ssl_context_t::verify_peer |
                                ssl_context_t::verify_fail_if_no_peer_cert);
        context.set_default_verify_paths();
        boost::certify::enable_native_https_server_verification(context);
    }

    void start_session();

   private:
    backend_t &backend_;
    resolver_t resolver_;

    boost::asio::ssl::stream<socket_t> frontend_stream_;
    beast::flat_buffer frontend_buffer_;
    // balancer <-- frontend
    http::request<http::string_body> frontend_request_;
    // balancer --> frontend
    http::response<http::string_body> frontend_response_;

    boost::asio::ssl::stream<socket_t> backend_stream_;
    beast::flat_buffer backend_buffer_;
    // balancer --> backend
    http::request<http::string_body> backend_request_;
    // balancer <-- backend
    http::response<http::string_body> backend_response_;

    // methods to handle frontend connection
    void frontend_read();

    void frontend_write();

    void frontend_on_read(beast::error_code error,
                          std::size_t bytes_transferred);

    void frontend_on_write(beast::error_code error,
                           std::size_t bytes_transferred);

    void handle_frontend_request(http::request<http::string_body> &&request);

    void frontend_close();

    // methods to handle backend connection
    void backend_read();

    void backend_write();

    void backend_on_read(beast::error_code error,
                         std::size_t bytes_transferred);

    void backend_on_write(beast::error_code error,
                          std::size_t bytes_transferred);

    void handle_backend_response(http::response<http::string_body> &&response);

    void backend_on_resolve(beast::error_code,
                            const resolver_t::results_type &);

    void backend_on_connect(beast::error_code ec);

    void backend_close();
};
}  // namespace lb::session

#endif  // LOAD_BALANCER_SSL_SESSION_HPP
