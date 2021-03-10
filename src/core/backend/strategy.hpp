#ifndef LOAD_BALANCER_STRATEGY_HPP
#define LOAD_BALANCER_STRATEGY_HPP

namespace lb::backend {
    enum class strategy {
        round_robin,
        least_connection
    };
}

#endif //LOAD_BALANCER_STRATEGY_HPP
