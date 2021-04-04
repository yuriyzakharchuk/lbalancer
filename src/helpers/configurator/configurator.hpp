//
// Created by yuriy on 2/10/21.
//

#ifndef LOAD_BALANCER_CONFIGURATOR_HPP
#define LOAD_BALANCER_CONFIGURATOR_HPP

#include <string>
#include <vector>
#include <yaml-cpp/yaml.h>

#include "../../core/backend/strategy.hpp"
#include "../../core/frontend/frontend.hpp"
#include "../../core/backend/backend_pool.hpp"
#include "../../core/workers/service_pool.hpp"
#include "../../core/meta/meta_server.hpp"
#include "../../core/meta/meta_backend.hpp"
#include "../../core/meta/meta_frontend.hpp"


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
        static constexpr const char* default_config_path = "с:\\lbalancer\\lbalancer.conf";
#endif

        // TODO: rename
        binding_t construct();

        [[nodiscard]] inline std::string
        default_ssl_certificate() const noexcept {
            return default_ssl_certificate_;
        }


        [[nodiscard]] inline std::string
        default_ssl_private_key() const noexcept {
            return default_ssl_private_key_;
        }


        [[nodiscard]] unsigned int
        worker_count() const noexcept {
            return worker_count_;
        }

        [[nodiscard]] inline bool
        use_ipc() const noexcept {
            return ipc_;
        }

        [[nodiscard]] inline bool
        use_threads() const noexcept {
            return !use_ipc();
        }


        [[nodiscard]] inline std::string
        error_log() const noexcept {
            return error_log_;
        }

        [[nodiscard]] inline std::string
        info_log() const noexcept {
            return info_log_;
        }


    private:
        unsigned int worker_count_ {};
        std::string  error_log_ {};
        std::string  info_log_ {};
        std::string  default_ssl_certificate_ {};
        std::string  default_ssl_private_key_ {};
        bool         ipc_ {};

        meta_frontend_pool meta_frontend_pool_;
        meta_backend_pool  meta_backend_pool_;

        // global
        void parse_worker_count(YAML::Node&);
        void parse_error_log(YAML::Node&);
        void parse_info_log(YAML::Node&);
        void parse_use_ipc(YAML::Node&);
        void parse_ssl_certificate(YAML::Node&);
        void parse_ssl_private_key(YAML::Node&);

        [[maybe_unused]] static std::string parse_ssl_tmp_dh(YAML::Node&);

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
