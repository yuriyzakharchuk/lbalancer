//
// Created by yuriy on 3/20/21.
//

#ifndef LOAD_BALANCER_MODE_HPP
#define LOAD_BALANCER_MODE_HPP

namespace lb::session {
    enum class mode {
        tcp,
        http,
        ssl
    };
}

#endif //LOAD_BALANCER_MODE_HPP
