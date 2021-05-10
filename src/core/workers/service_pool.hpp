#ifndef LOAD_BALANCER_SERVICE_POOL_HPP
#define LOAD_BALANCER_SERVICE_POOL_HPP

#include <boost/asio.hpp>
#include <cctype>
#include <memory>
#include <vector>

namespace lb::workers {

class service_pool {
   public:
    using service_t = boost::asio::io_context;

    explicit service_pool(std::size_t pool_size);

    service_pool(service_pool&&) = delete;
    service_pool(const service_pool&) = delete;
    service_pool& operator=(service_pool&&) = delete;
    service_pool& operator=(const service_pool&) = delete;

    void run_all();

    inline service_t& service() {
        return io_service_;
    }

   private:
    std::size_t pool_size_;
    service_t io_service_;
};

}  // namespace lb::workers

#endif  // LOAD_BALANCER_SERVICE_POOL_HPP
