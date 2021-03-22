#ifndef LOAD_BALANCER_SERVER_HPP
#define LOAD_BALANCER_SERVER_HPP

#include <string>

#include "../workers/service_pool.hpp"
#include "../backend/backend_pool.hpp"
#include "../../helpers/configurator.hpp"


namespace lb {
    class server {
    public:
        explicit server(helpers::configurator& configurator);

        server(server&&) = delete;
        server(const server&) = delete;
        server& operator=(server&&) = delete;
        server& operator=(const server&) = delete;

        void run();

    private:
        workers::service_pool service_pool_;
        boost::asio::signal_set signals_; //TODO: signal handling
    };
} //lb


#endif //LOAD_BALANCER_SERVER_HPP
