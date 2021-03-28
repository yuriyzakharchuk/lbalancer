//
// Created by yuriy on 3/21/21.
//

#ifndef LOAD_BALANCER_META_BACKEND_HPP
#define LOAD_BALANCER_META_BACKEND_HPP

#include <vector>
#include "meta_server.hpp"
#include "../session/mode.hpp"
#include "../backend/strategy.hpp"


namespace lb::helpers {
    struct meta_backend {
        std::string               name {};
        backend::strategy         strategy {};
        std::vector<meta_server>  pool {};
    };

    using meta_backend_pool = std::vector<meta_backend>;
}

#endif //LOAD_BALANCER_META_BACKEND_HPP
