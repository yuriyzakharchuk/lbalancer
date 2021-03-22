#include "backend_pool.hpp"


lb::backend::backend_pool::backend_pool(const lb::helpers::meta_backend& meta_backend)
    : balance_(meta_backend.strategy),
      pool_() {
    pool_.reserve(meta_backend.pool.size());
    for(auto& meta_server : meta_backend.pool) {
        auto server = backend {
            meta_server.address,
            meta_server.port,
            meta_server.weight,
            meta_server.is_backup
        };
        pool_.push_back(server);
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

