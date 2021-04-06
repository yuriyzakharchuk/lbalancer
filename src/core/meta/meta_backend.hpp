#ifndef LOAD_BALANCER_META_BACKEND_HPP
#define LOAD_BALANCER_META_BACKEND_HPP

#include <vector>

#include "../backend/strategy.hpp"
#include "../session/mode.hpp"
#include "meta_server.hpp"

namespace lb::helpers {
struct meta_backend {
    std::string name{};
    backend::strategy strategy{};
    std::vector<meta_server> pool{};
};

using meta_backend_pool = std::vector<meta_backend>;
}  // namespace lb::helpers

#endif  // LOAD_BALANCER_META_BACKEND_HPP
