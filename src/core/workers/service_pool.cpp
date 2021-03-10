#include <stdexcept>
#include <thread>
#include <utility>

#include "service_pool.hpp"


lb::workers::service_pool::service_pool(std::size_t pool_size)
        : next_service_(0) {
    if (pool_size == 0) {
        throw std::runtime_error("Service pool can't be empty.");
    }
    for (std::size_t i = 0; i < pool_size; ++i) {
        std::shared_ptr<boost::asio::io_service> service {
                new boost::asio::io_service()
        };
        std::shared_ptr<boost::asio::io_service::work> work {
                new boost::asio::io_service::work(*service)
        };
        pool_.push_back(service);
        work_.push_back(work);
    }
}


void
lb::workers::service_pool::run_all() {
    for (auto &service : pool_) {
        thread_pool_.push_back(std::make_shared<std::thread>([&service]() {
            service->run();
            // TODO: log if service stopped
        }));
    }
    for (auto &thread : thread_pool_) {
        thread->join();
    }
}


boost::asio::io_service &
lb::workers::service_pool::get_service() {
    boost::asio::io_service &s = { *pool_.at(next_service_) };
    ++next_service_;
    if (next_service_ == pool_.size()) {
        next_service_ = 0;
    }
    return s;
}

