#ifndef LOAD_BALANCER_SERVER_HPP
#define LOAD_BALANCER_SERVER_HPP

#include <string>

#include "../workers/service_pool.hpp"
#include "../backend/backend_pool.hpp"
#include "../../helpers/configurator.hpp"


namespace lb {
    class server {
    public:
        server(server&&) = delete;
        server(const server&) = delete;
        server& operator=(server&&) = delete;
        server& operator=(const server&) = delete;

        explicit server(helpers::configurator& configurator)
            : service_pool_(configurator.worker_count()),
            signals_(service_pool_.service()) {
            for(const auto& binding : configurator.construct()) {
                std::make_shared<frontend::frontend>(binding.first,
                                                     service_pool_.service(),
                                                     std::move(backend::backend_pool(binding.second)))->start_accept();
            }
        }

        void run() {
            service_pool_.run_all();
        }

    private:
        workers::service_pool service_pool_;
        boost::asio::signal_set signals_; //TODO: signal handling
    };
} //lb


#endif //LOAD_BALANCER_SERVER_HPP
