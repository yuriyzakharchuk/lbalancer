#ifndef LOAD_BALANCER_BACKEND_HPP
#define LOAD_BALANCER_BACKEND_HPP

#include <boost/asio.hpp>
#include <string>

namespace lb::backend {
class backend {
   public:
    explicit backend(std::string server, std::string port, unsigned int weight,
                     bool is_backup);

    std::string &server();
    std::string &port();

   private:
    std::string server_;
    std::string port_;
    [[maybe_unused]] unsigned int weight_;
    [[maybe_unused]] bool is_backup_;
};
}  // namespace lb::backend

#endif  // LOAD_BALANCER_BACKEND_HPP
