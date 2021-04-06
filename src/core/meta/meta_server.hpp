#ifndef LOAD_BALANCER_META_SERVER_HPP
#define LOAD_BALANCER_META_SERVER_HPP

namespace lb::helpers {
struct meta_server {
    [[maybe_unused]] std::string name{};
    std::string address{};
    std::string port{};
    unsigned int weight{};
    bool is_backup{};
};

using meta_server_pool = std::vector<meta_server>;
}  // namespace lb::helpers

#endif  // LOAD_BALANCER_META_SERVER_HPP
