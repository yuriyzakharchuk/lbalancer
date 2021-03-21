//
// Created by yuriy on 2/10/21.
//

#include <iostream>
#include "configurator.hpp"


lb::helpers::configurator::configurator(int argc, char **argv) {
    try {
        YAML::Node config = YAML::LoadFile(default_config_path);
        if(YAML::Node global = config["global"]) {
            parse_worker_count(global);
            parse_error_log(global);
            parse_info_log(global);
            parse_use_ipc(global);
        }
        else {
            throw std::runtime_error("Config: global section not found");
        }
        if(YAML::Node frontends = config["frontends"]) {

        }
        else {
            throw std::runtime_error("Config: frontends section not found");
        }
    }
    catch (const std::runtime_error& e) {
        std::cerr << e.what() << std::endl;
        std::exit(1);
    }
}


unsigned int
lb::helpers::configurator::worker_count() const noexcept {
    return worker_count_;
}


bool
lb::helpers::configurator::use_ipc() const noexcept {
    return ipc_;
}


bool
lb::helpers::configurator::use_threads() const noexcept {
    return !use_ipc();
}


std::string
lb::helpers::configurator::error_log() const noexcept {
    return error_log_;
}


std::string
lb::helpers::configurator::info_log() const noexcept {
    return info_log_;
}


lb::helpers::configurator::frontend_pool_t
lb::helpers::configurator::construct(workers::service_pool& service_pool) {
    for (int i = 0; i < 1; ++i) {
        // TODO: refactor that shit
        std::vector<std::pair<std::string, std::string>> servers;
        servers.emplace_back(std::pair("127.0.0.1", "8000"));

        // TODO: session mode handling
        auto f = lb::frontend::frontend{
                "0.0.0.0",
                "5000",
                service_pool,
                lb::backend::backend_pool{
                        lb::backend::strategy::round_robin,
                        servers,
                        service_pool.get_service(),
                        lb::session::mode::tcp
                },
                lb::session::mode::tcp
        };
        frontend_pool_.emplace_back(std::move(f));
    }
    return std::move(frontend_pool_);
}


void
lb::helpers::configurator::parse_worker_count(YAML::Node& global) {
    if(auto node = global["worker_process"]) {
        auto workers = node.as<std::string>();
        if (workers == "auto") {
            worker_count_ = cpu_count();
        }
        else {
            try {
                worker_count_ = std::stoi(workers);
            }
            catch(const std::invalid_argument& e) {
                std::cerr << e.what() << std::endl;
                worker_count_ = cpu_count();
            }
            catch (const std::exception& e) {
                std::cerr << e.what() << std::endl;
                std::exit(1);
            }
        }
    }
    else {
        std::cout << "Config: no worker_process parameter found." << std::endl;
        std::exit(1);
    }
}


void
lb::helpers::configurator::parse_error_log(YAML::Node& global) {
    if(auto node = global["error_log"]) {
        auto path = node.as<std::string>();
        // TODO: validate path
        error_log_ = path;
    }
    else {
        // TODO: default path of log OR off error logging
        error_log_.clear();
    }
}


void
lb::helpers::configurator::parse_info_log(YAML::Node& global) {
    if(auto node = global["info_log"]) {
        auto path = node.as<std::string>();
        // TODO: validate path
        info_log_ = path;
    }
    else {
        // TODO: default path of log OR off error logging
        info_log_.clear();
    }
}


void
lb::helpers::configurator::parse_use_ipc(YAML::Node& global) {
    if (auto value = global["use_ipc"]) {
        ipc_ = global["use_ipc"].as<bool>();
    }
    else {
        ipc_ = false;
    }
}

