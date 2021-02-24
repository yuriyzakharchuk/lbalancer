#ifndef LOAD_BALANCER_SERVER_HPP
#define LOAD_BALANCER_SERVER_HPP

#include <string>

#include "../workers/service_pool.hpp"
#include "../frontend/frontend.hpp"
#include "../backend/backend_pool.hpp"


namespace lb {
    class server {
    public:
        explicit server(const std::string &,
                        const std::string &,
                        std::size_t num_threads);

        server(const server &) = delete;

        server &
        operator=(const server &) = delete;

        void
        run();

    private:
        workers::service_pool service_pool_;
        boost::asio::signal_set signals_; //TODO: signal handling
        boost::asio::ip::tcp::acceptor acceptor_;
        std::vector<lb::frontend::frontend> frontend_pool_;

        void
        start_accept();
    };
} //lb


#endif //LOAD_BALANCER_SERVER_HPP
