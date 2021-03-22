#ifndef LOAD_BALANCER_SERVICE_POOL_HPP
#define LOAD_BALANCER_SERVICE_POOL_HPP

#include <cctype>
#include <vector>
#include <memory>

#include <boost/asio.hpp>

namespace lb::workers {

    class service_pool {
    public:
        using service_t      = boost::asio::io_context;
        using thread_pool_t  = std::vector<std::shared_ptr<std::thread>>;
        using service_pool_t = std::vector<std::shared_ptr<boost::asio::io_context>>;
        using service_work_t = std::vector<std::shared_ptr<boost::asio::io_context::work>>;

        explicit service_pool(std::size_t pool_size);

        service_pool(service_pool&&) = delete;
        service_pool(const service_pool&) = delete;
        service_pool& operator=(service_pool&&) = delete;
        service_pool& operator=(const service_pool&) = delete;

        void run_all();

        service_t& service();

    private:
        std::size_t next_service_;

        service_pool_t pool_;
        thread_pool_t  thread_pool_;
        service_work_t work_;
    };

} // lb::workers


#endif //LOAD_BALANCER_SERVICE_POOL_HPP
