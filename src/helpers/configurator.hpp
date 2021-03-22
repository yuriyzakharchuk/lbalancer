//
// Created by yuriy on 2/10/21.
//

#ifndef LOAD_BALANCER_CONFIGURATOR_HPP
#define LOAD_BALANCER_CONFIGURATOR_HPP

#include <string>
#include <vector>
#include <yaml-cpp/yaml.h>

#include "../core/backend/strategy.hpp"
#include "../core/frontend/frontend.hpp"
#include "../core/backend/backend_pool.hpp"
#include "../core/workers/service_pool.hpp"

#include "meta/meta_server.hpp"
#include "meta/meta_backend.hpp"
#include "meta/meta_frontend.hpp"


namespace lb::helpers {
    class configurator final {
    public:
        using binding_t = std::vector<std::pair<meta_frontend, meta_backend>>;

        configurator(int argc, char **argv);

        configurator(configurator&&) = delete;
        configurator(const configurator&) = delete;
        configurator& operator=(configurator&&) = delete;
        configurator& operator=(const configurator&) = delete;

        static inline auto
        cpu_count() noexcept {
            return std::thread::hardware_concurrency();
        }


#if defined(__linux__)
        static constexpr const char* default_config_path = "/etc/lbalancer/lbalancer.conf";
#elif defined(_WIN64)
        // TODO: where store windows config
        static constexpr char* default_config_path = "c:/lbalancer/lbalancer.conf";
#endif

        // TODO: rename
        binding_t construct();

        [[nodiscard]] unsigned int worker_count() const noexcept;

        [[nodiscard]] inline bool use_ipc() const noexcept;

        [[nodiscard]] inline bool use_threads() const noexcept;

        [[nodiscard]] inline std::string error_log() const noexcept;

        [[nodiscard]] inline std::string info_log() const noexcept;


    private:
        unsigned int worker_count_ {};
        std::string  error_log_ {};
        std::string  info_log_ {};
        bool         ipc_ {};

        meta_frontend_pool meta_frontend_pool_;
        meta_backend_pool  meta_backend_pool_;

        // global
        void parse_worker_count(YAML::Node&);
        void parse_error_log(YAML::Node&);
        void parse_info_log(YAML::Node&);
        void parse_use_ipc(YAML::Node&);

        // frontends section
        void parse_frontends(YAML::Node&);

        // backends section
        void parse_backends(YAML::Node&);
        static meta_server_pool parse_servers(YAML::Node&);

        // helpers
        static unsigned int parse_weight(YAML::Node&);
        static bool parse_is_backup(YAML::Node&);
        static std::string parse_port(YAML::Node&);
        static std::string parse_address(YAML::Node&);
        static session::mode parse_mode(YAML::Node&);
        static backend::strategy parse_balance(YAML::Node&);
        static std::string parse_backend(YAML::Node&);
    };
}

#endif //LOAD_BALANCER_CONFIGURATOR_HPP
