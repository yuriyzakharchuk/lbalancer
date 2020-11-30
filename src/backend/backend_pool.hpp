#ifndef LOAD_BALANCER_BACKEND_POOL_HPP
#define LOAD_BALANCER_BACKEND_POOL_HPP

#include <vector>
#include <boost/asio.hpp>

#include "backend.hpp"
#include "strategy.hpp"


namespace lb::backend {
    class backend_pool {
    public:
        using server_t = std::vector<std::pair<std::string, std::string>>;

        explicit backend_pool(strategy,
                              const server_t &,
                              boost::asio::io_service &);

        lb::backend::backend &
        next_backend();

    private:
        int current_backend_ = 0;
        std::vector<backend> pool_;
    };
}


#endif //LOAD_BALANCER_BACKEND_POOL_HPP
