#ifndef LOAD_BALANCER_BACKEND_HPP
#define LOAD_BALANCER_BACKEND_HPP

#include <string>
#include <boost/asio.hpp>

namespace lb::backend {
    class backend {
    public:
        explicit backend(std::string server,
                         std::string port,
                         unsigned int weight,
                         bool is_backup);

        std::string& server();

        std::string& port();

    private:
        std::string  server_;
        std::string  port_;
        unsigned int weight_;
        bool         is_backup_;
    };
} //lb::backend


#endif //LOAD_BALANCER_BACKEND_HPP
