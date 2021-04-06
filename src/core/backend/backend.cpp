#include "backend.hpp"

#include <iostream>
#include <utility>

lb::backend::backend::backend(std::string server, std::string port,
                              const unsigned int weight, const bool is_backup)
    : server_(std::move(server)),
      port_(std::move(port)),
      weight_(weight),
      is_backup_(is_backup) {}

std::string &lb::backend::backend::server() { return server_; }

std::string &lb::backend::backend::port() { return port_; }
