#ifndef LOAD_BALANCER_FRONTEND_HPP
#define LOAD_BALANCER_FRONTEND_HPP

#include <memory>
#include <string>
#include <boost/asio.hpp>

#include "../backend/backend_pool.hpp"
#include "../session/session.hpp"
#include "../workers/service_pool.hpp"


namespace lb::frontend {
    class frontend {
    public:
        frontend(workers::service_pool &,
                 boost::asio::ip::tcp::acceptor &,
                 lb::backend::backend_pool);

        void
        start_accept();

    private:
        workers::service_pool &service_pool_;
        boost::asio::ip::tcp::acceptor &acceptor_;
        lb::backend::backend_pool backend_pool_;
        std::shared_ptr<session::session> new_session_;
        lb::session::session_handler session_handler_;

        void
        handle_accept(const boost::system::error_code &);
    };
}


#endif //LOAD_BALANCER_FRONTEND_HPP
