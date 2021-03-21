#ifndef LOAD_BALANCER_SERVER_HPP
#define LOAD_BALANCER_SERVER_HPP

#include <string>

#include "../workers/service_pool.hpp"
#include "../backend/backend_pool.hpp"
#include "../session/basic_session.hpp"
#include "../../helpers/configurator.hpp"


namespace lb {
    class server {
    public:
        using frontend_pool_t = helpers::configurator::frontend_pool_t;

        explicit server(helpers::configurator& configurator);

        server(const server &) = delete;
        server& operator=(const server &) = delete;

        void run();

    private:
        workers::service_pool service_pool_;
        frontend_pool_t frontend_pool_;
        boost::asio::signal_set signals_; //TODO: signal handling

        void start_accept();
    };
} //lb


#endif //LOAD_BALANCER_SERVER_HPP
