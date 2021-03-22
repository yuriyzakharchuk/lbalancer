#ifndef LOAD_BALANCER_FRONTEND_HPP
#define LOAD_BALANCER_FRONTEND_HPP

#include <memory>
#include <string>
#include <boost/asio.hpp>

#include "../session/mode.hpp"
#include "../session/tcp/tcp_session.hpp"
#include "../session/http/http_session.hpp"
#include "../session/ssl/ssl_session.hpp"

#include "../backend/backend_pool.hpp"
#include "../workers/service_pool.hpp"
#include "../../helpers/meta/meta_frontend.hpp"


namespace lb::frontend {
    class frontend : public std::enable_shared_from_this<frontend> {
    public:
        using socket_t = boost::asio::ip::tcp::socket;

        frontend(const helpers::meta_frontend&,
                 workers::service_pool::service_t &,
                 backend::backend_pool&&);

        void
        start_accept();

    private:
        session::mode mode_;
        backend::backend_pool backend_pool_;
        boost::asio::ip::tcp::acceptor acceptor_;
        workers::service_pool::service_t &service_;

        void handle_accept(const boost::system::error_code&, socket_t socket);
    };
}


#endif //LOAD_BALANCER_FRONTEND_HPP
