#ifndef LOAD_BALANCER_FRONTEND_HPP
#define LOAD_BALANCER_FRONTEND_HPP

#include <boost/asio.hpp>
#include <memory>
#include <string>

#include "../backend/backend_pool.hpp"
#include "../meta/meta_frontend.hpp"
#include "../session/http/http_session.hpp"
#include "../session/mode.hpp"
#include "../session/ssl/ssl_session.hpp"
#include "../session/tcp/tcp_session.hpp"
#include "../workers/service_pool.hpp"

namespace lb::frontend {
class frontend : public std::enable_shared_from_this<frontend> {
   public:
    using socket_t = boost::asio::ip::tcp::socket;

    frontend(const helpers::meta_frontend &, workers::service_pool::service_t &,
             boost::asio::ssl::context &, boost::asio::ssl::context &,
             backend::backend_pool &&);

    void start_accept();

   private:
    session::mode mode_;
    backend::backend_pool backend_pool_;
    boost::asio::ip::tcp::acceptor acceptor_;
    workers::service_pool::service_t &service_;
    boost::asio::ssl::context &frontend_context_;
    boost::asio::ssl::context &backend_context_;

    void handle_accept(const boost::system::error_code &, socket_t socket);
};
}  // namespace lb::frontend

#endif  // LOAD_BALANCER_FRONTEND_HPP
