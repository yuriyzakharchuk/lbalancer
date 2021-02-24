#include <iostream>
#include <utility>

#include "backend.hpp"


lb::backend::backend::backend(std::string server,
                              std::string port,
                              boost::asio::io_service &service)
        : server_(std::move(server)), port_(std::move(port)) {
}


std::string &
lb::backend::backend::server() {
    return server_;
}


std::string &
lb::backend::backend::port() {
    return port_;
}

