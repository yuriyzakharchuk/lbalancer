#ifndef LOAD_BALANCER_SERVER_HPP
#define LOAD_BALANCER_SERVER_HPP

#include <string>

#include "../workers/service_pool.hpp"
#include "../backend/backend_pool.hpp"
#include "../session/ssl/ssl_session.hpp"
#include "../../helpers/configurator/configurator.hpp"


namespace lb {
    class server {
    public:
        server(server&&) = delete;
        server(const server&) = delete;
        server& operator=(server&&) = delete;
        server& operator=(const server&) = delete;

        explicit
        server(helpers::configurator& configurator)
            : service_pool_(static_cast<size_t>(configurator.worker_count())),
              frontend_ssl_context_(boost::asio::ssl::context::tls_server),
              backend_ssl_context_(boost::asio::ssl::context::tls_client),
              signals_(service_pool_.service()) {

            frontend_ssl_context_.set_options(boost::asio::ssl::context::default_workarounds |
                                                 boost::asio::ssl::context::no_sslv2 |
                                                 boost::asio::ssl::context::no_sslv3 |
                                                 boost::asio::ssl::context::no_tlsv1 |
                                                 boost::asio::ssl::context::no_tlsv1_1 |
                                                 boost::asio::ssl::context::no_tlsv1_2);

            session::ssl_session::verify_certificate(backend_ssl_context_);
            session::ssl_session::load_server_certificate(frontend_ssl_context_,
                                                          configurator.default_ssl_certificate(),
                                                          configurator.default_ssl_private_key());


            for(const auto& binding : configurator.construct()) {
                std::make_shared<frontend::frontend>(binding.first,
                                                     service_pool_.service(),
                                                     frontend_ssl_context_,
                                                     backend_ssl_context_,
                                                     backend::backend_pool(binding.second))->start_accept();
            }
        }

        void run() {
            service_pool_.run_all();
        }

    private:
        workers::service_pool service_pool_;
        boost::asio::ssl::context frontend_ssl_context_;
        boost::asio::ssl::context backend_ssl_context_;
        [[maybe_unused]] boost::asio::signal_set signals_; //TODO: signal handling
    };
} //lb


#endif //LOAD_BALANCER_SERVER_HPP
