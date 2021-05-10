#include "service_pool.hpp"

#include <stdexcept>
#include <thread>
#include <boost/bind.hpp>
#include <utility>

lb::workers::service_pool::service_pool(std::size_t pool_size) : pool_size_(pool_size){

}

void lb::workers::service_pool::run_all() {
    std::vector<std::shared_ptr<std::thread> > threads;
    for (std::size_t i = 0; i < pool_size_; ++i)
    {
        std::shared_ptr<std::thread> thread(new std::thread(
            boost::bind(&boost::asio::io_service::run, &io_service_)));
        threads.push_back(thread);
    }

    // Wait for all threads in the pool to exit.
    for (std::size_t i = 0; i < threads.size(); ++i)
        threads[i]->join();
}

