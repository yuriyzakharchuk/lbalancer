#ifndef LOAD_BALANCER_META_FRONTEND_HPP
#define LOAD_BALANCER_META_FRONTEND_HPP

#include <string>

#include "../session/mode.hpp"

namespace lb::helpers {
struct meta_frontend {
    [[maybe_unused]] std::string name;
    std::string address;
    std::string port;
    std::string backend;
    session::mode mode;
};

using meta_frontend_pool = std::vector<meta_frontend>;
}  // namespace lb::helpers

#endif  // LOAD_BALANCER_META_FRONTEND_HPP
