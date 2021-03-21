#include "server.hpp"


lb::server::server(helpers::configurator& configurator)
        : service_pool_(configurator.worker_count()),
          frontend_pool_(configurator.construct(service_pool_)),
          signals_(service_pool_.get_service()) {
    start_accept();
}


void
lb::server::start_accept() {
    for (auto &frontend : frontend_pool_) {
        frontend.start_accept();
    }
}


void
lb::server::run() {
    service_pool_.run_all();
}

