#ifndef LOAD_BALANCER_HTTP_SESSION_HPP
#define LOAD_BALANCER_HTTP_SESSION_HPP

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>

#include "../../backend/backend.hpp"

namespace beast = boost::beast;
namespace http = beast::http;

namespace lb::session {
class http_session : public std::enable_shared_from_this<http_session> {
   public:
    using socket_t = boost::asio::ip::tcp::socket;
    using resolver_t = boost::asio::ip::tcp::resolver;
    using backend_t = backend::backend;

    explicit http_session(socket_t&&, backend_t&);

    void start_session();

   private:
    backend_t& backend_;

    beast::tcp_stream frontend_stream_;
    beast::flat_buffer frontend_buffer_;
    // balancer <-- frontend
    http::request<http::string_body> frontend_request_;
    // balancer --> frontend
    http::response<http::string_body> frontend_response_;

    beast::tcp_stream backend_stream_;
    beast::flat_buffer backend_buffer_;
    // balancer --> backend
    http::request<http::string_body> backend_request_;
    // balancer <-- backend
    http::response<http::string_body> backend_response_;

    void frontend_read();
    void frontend_write();
    void frontend_on_read(beast::error_code ec, std::size_t bytes_transferred);
    void frontend_on_write(beast::error_code ec, std::size_t bytes_transferred);
    void handle_frontend_request(http::request<http::string_body>&& request);
    void frontend_close();

    void backend_read();
    void backend_write();
    void backend_on_read(beast::error_code ec, std::size_t bytes_transferred);
    void backend_on_write(beast::error_code ec, std::size_t bytes_transferred);
    void handle_backend_response(http::response<http::string_body>&& response);
    void backend_close();
};
}  // namespace lb::session

#endif  // LOAD_BALANCER_HTTP_SESSION_HPP
