//
// Created by yuriy on 2/10/21.
//

#include <iostream>
#include "configurator.hpp"

using namespace lb::helpers;

configurator::configurator(int argc, char **argv) {
    // TODO: parse args

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

        if(YAML::Node backends = config["backends"]) {
            parse_backends(backends);
        }
        else {
            throw std::runtime_error("Config: backends section not found");
        }

        if(YAML::Node frontends = config["frontends"]) {
            parse_frontends(frontends);
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
configurator::worker_count() const noexcept {
    return worker_count_;
}


bool
configurator::use_ipc() const noexcept {
    return ipc_;
}


bool
configurator::use_threads() const noexcept {
    return !use_ipc();
}


std::string
configurator::error_log() const noexcept {
    return error_log_;
}


std::string
configurator::info_log() const noexcept {
    return info_log_;
}


configurator::frontend_pool_t
configurator::construct(workers::service_pool& service_pool) {
    frontend_pool_t frontend_pool {};
    frontend_pool.reserve(meta_frontend_pool_.size());

    for(auto& meta_frontend : meta_frontend_pool_) {
        // creating backend pool for current meta frontend
        auto it = std::find_if(meta_backend_pool_.begin(), meta_backend_pool_.end(),
                               [&meta_frontend](const meta_backend& m) { return m.name == meta_frontend.backend; });
        if(it == meta_backend_pool_.end()) {
            std::exit(1);
        }
        backend::backend_pool backend_pool { *it };

        // creating frontend
        frontend_pool.emplace_back(meta_frontend, service_pool, backend_pool);
    }
    return frontend_pool;
}


void
configurator::parse_worker_count(YAML::Node& global) {
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
configurator::parse_error_log(YAML::Node& global) {
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
configurator::parse_info_log(YAML::Node& global) {
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
configurator::parse_use_ipc(YAML::Node& global) {
    if (global["use_ipc"]) {
        ipc_ = global["use_ipc"].as<bool>();
    }
    else {
        ipc_ = false;
    }
}


void
configurator::parse_backends(YAML::Node& node) {
    for(YAML::iterator it = node.begin(); it != node.end(); ++it) {
        auto key { it->first };
        auto value { it->second };
        meta_backend_pool_.emplace_back(meta_backend {
                key.as<std::string>(),
                parse_balance(value),
                parse_mode(value),
                parse_servers(value)
        });
    }
}


void
configurator::parse_frontends(YAML::Node& frontends) {
    for(YAML::iterator it = frontends.begin(); it != frontends.end(); ++it) {
        meta_frontend_pool_.emplace_back(meta_frontend {
                it->first.as<std::string>(),
                parse_address(it->second),
                parse_port(it->second),
                parse_backend(it->second),
                parse_mode(it->second)
        });
    }
}


std::string
configurator::parse_port(YAML::Node& node) {
    try{
        if (auto value = node["port"]) {
            return value.as<std::string>();
        }
        else {
            return "0";
        }
    }
    catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
        std::exit(1);
    }
}


std::string
configurator::parse_address(YAML::Node& node) {
    try{
        if (auto value = node["address"]) {
            return value.as<std::string>();
        }
        else {
            return "";
        }
    }
    catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
        std::exit(1);
    }
}


lb::session::mode
configurator::parse_mode(YAML::Node &node) {
    try{
        if (auto value = node["mode"]) {
            auto mode = value.as<std::string>();
            if (mode == "http") {
                return lb::session::mode::http;
            }
            else if (mode == "ssl") {
                return lb::session::mode::ssl;
            }
            else {
                return lb::session::mode::tcp;
            }
        }
        else {
            return lb::session::mode::tcp;
        }
    }
    catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
        std::exit(1);
    }
}


lb::backend::strategy
configurator::parse_balance(YAML::Node& node) {
    try {
        if (auto value = node["balance"]) {
            auto mode = value.as<std::string>();
            return lb::backend::strategy::round_robin;
        }
        else {
            return lb::backend::strategy::round_robin;
        }
    }
    catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
        std::exit(1);
    }
}


std::string
configurator::parse_backend(YAML::Node& node) {
    try{
        if (auto value = node["backend"]) {
            return value.as<std::string>();
        }
        else {
            throw std::runtime_error("Config: can't parse backend value");
        }
    }
    catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
        std::exit(1);
    }
}


meta_server_pool
configurator::parse_servers(YAML::Node& node) {
    meta_server_pool servers {};
    servers.reserve(node.size());

    auto servers_map = node["servers"];
    for(YAML::iterator it = servers_map.begin(); it != servers_map.end(); ++it) {
        auto key { it->first };
        auto value { it->second };
        servers.emplace_back(meta_server {
                key.as<std::string>(),
                parse_address(value),
                parse_port(value),
                parse_weight(value),
                parse_is_backup(value)
        });
    }
    return servers;
}


unsigned int
configurator::parse_weight(YAML::Node& node) {
    try{
        if (auto value = node["weight"]) {
            return value.as<unsigned int>();
        }
        else {
            return 0;
        }
    }
    catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
        std::exit(1);
    }
}


bool
configurator::parse_is_backup(YAML::Node& node) {
    if (node["is_backup"]) {
        return node["is_backup"].as<bool>();
    }
    else {
        return false;
    }
}

