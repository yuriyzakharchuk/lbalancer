//
// Created by yuriy on 3/10/21.
//

#ifndef LOAD_BALANCER_BASIC_LOGGER_HPP
#define LOAD_BALANCER_BASIC_LOGGER_HPP

#include <string>

namespace lb {

    enum class LOG_CODE {
        LOG_TRACE,
        LOG_INFO,
        LOG_WARNING,
        LOG_ERROR
    };

    void
    log_init();

    void
    console_write(const std::string& message);

    void
    log_error(const std::string& message);

    void
    log_info(const std::string& message);

    void
    log_trace(const std::string& message);
}

#endif //LOAD_BALANCER_BASIC_LOGGER_HPP
