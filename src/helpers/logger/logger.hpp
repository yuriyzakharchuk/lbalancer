//
// Created by yuriy on 3/20/21.
//

#ifndef LOAD_BALANCER_LOGGER_HPP
#define LOAD_BALANCER_LOGGER_HPP


#include "basic_logger.hpp"

#ifdef NO_LOG_ERROR
#define log_error() (void)0
#endif

#ifdef NO_LOG_INFO
#define log_info() (void)0
#endif

#ifdef NO_LOG_TRACE
#define log_trace() (void)0
#endif


#endif //LOAD_BALANCER_LOGGER_HPP
