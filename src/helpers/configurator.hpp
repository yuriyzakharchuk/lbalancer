//
// Created by yuriy on 2/10/21.
//

#ifndef LOAD_BALANCER_CONFIGURATOR_HPP
#define LOAD_BALANCER_CONFIGURATOR_HPP

#include <string>
#include <vector>
#include <yaml-cpp/yaml.h>
#include "../core/frontend/frontend.hpp"
#include "../core/workers/service_pool.hpp"


namespace lb::helpers {
    class configurator final {
    public:
        using frontend_pool_t = std::vector<lb::frontend::frontend>;

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

        frontend_pool_t
        construct(workers::service_pool& service_pool);


        [[nodiscard]] unsigned int
        worker_count() const noexcept;


        [[nodiscard]] inline bool
        use_ipc() const noexcept;


        [[nodiscard]] inline bool
        use_threads() const noexcept;


        [[nodiscard]] inline std::string
        error_log() const noexcept;


        [[nodiscard]] inline std::string
        info_log() const noexcept;


    private:
        unsigned int worker_count_ {};
        std::string  error_log_ {};
        std::string  info_log_ {};
        bool         ipc_ {};

        frontend_pool_t frontend_pool_;

        // global
        void
        parse_worker_count(YAML::Node&);

        void
        parse_error_log(YAML::Node&);

        void
        parse_info_log(YAML::Node&);

        void
        parse_use_ipc(YAML::Node&);
    };
}

#endif //LOAD_BALANCER_CONFIGURATOR_HPP
