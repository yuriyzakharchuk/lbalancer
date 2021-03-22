#include "server.hpp"


lb::server::server(helpers::configurator& configurator)
        : service_pool_(configurator.worker_count()),
          signals_(service_pool_.service()) {

    for(const auto& binding : configurator.construct()) {
        std::make_shared<frontend::frontend>(binding.first,
                                             service_pool_.service(),
                                             std::move(backend::backend_pool(binding.second)))->start_accept();
    }
}


void
lb::server::run() {
    service_pool_.run_all();
}

