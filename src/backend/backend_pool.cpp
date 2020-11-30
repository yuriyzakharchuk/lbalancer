#include "backend_pool.hpp"


lb::backend::backend_pool::backend_pool(lb::backend::strategy,
                                        const server_t & servers,
                                        boost::asio::io_service &service) {
    // TODO: correct creating pool
    // TODO: strategy
    for(auto &server : servers) {
        pool_.emplace_back(server.first, server.second, service);
    }
}


lb::backend::backend&
lb::backend::backend_pool::next_backend() {
    auto &backend { pool_[current_backend_] };
    ++current_backend_;
    if(current_backend_ == pool_.size()) {
        current_backend_ = 0;
    }
    return backend;
}
