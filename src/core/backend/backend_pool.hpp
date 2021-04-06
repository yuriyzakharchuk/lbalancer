#ifndef LOAD_BALANCER_BACKEND_POOL_HPP
#define LOAD_BALANCER_BACKEND_POOL_HPP

#include <vector>
#include <boost/asio.hpp>

#include "backend.hpp"
#include "strategy.hpp"
#include "../session/mode.hpp"
#include "../meta/meta_backend.hpp"


namespace lb::backend {
    class backend_pool {
    public:
        explicit backend_pool(const helpers::meta_backend&);

        backend& next_backend();

    private:
        [[maybe_unused]] const strategy balance_;
        std::vector<backend> pool_;

        std::size_t current_backend_ = 0;
    };
}


#endif //LOAD_BALANCER_BACKEND_POOL_HPP
