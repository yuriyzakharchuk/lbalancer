#include "frontend.hpp"

using namespace lb::frontend;
using namespace lb::backend;
using namespace lb::helpers;
using namespace lb::workers;
using namespace lb::session;


frontend::frontend(const meta_frontend& meta_frontend,
                   service_pool::service_t& service,
                   backend_pool&& backend_pool)
        : service_(service),
          acceptor_(boost::asio::make_strand(service)),
          backend_pool_(std::move(backend_pool)),
          mode_(meta_frontend.mode) {

    using boost::asio::ip::tcp;

    tcp::resolver resolver(acceptor_.get_executor());
    tcp::endpoint endpoint { *resolver.resolve(std::string_view(meta_frontend.address),
                                               std::string_view(meta_frontend.port)) };

    // TODO: add error handling
    boost::system::error_code error_code;
    acceptor_.open(endpoint.protocol(), error_code);
    if(error_code) {

    }
    acceptor_.set_option(tcp::acceptor::reuse_address(true), error_code);
    if(error_code) {

    }
    acceptor_.bind(endpoint, error_code);
    if(error_code) {

    }
    acceptor_.listen(boost::asio::socket_base::max_listen_connections, error_code);
    if(error_code) {

    }
}


void frontend::start_accept() {
    acceptor_.async_accept(boost::asio::make_strand(service_),
            beast::bind_front_handler(
                    &frontend::handle_accept,
                    shared_from_this()));
}


void frontend::handle_accept(const boost::system::error_code &error,
                                      boost::asio::ip::tcp::socket frontend_socket) {
    if (!error) {
        switch (mode_) {
            case mode::tcp: {
                std::make_shared<tcp_session>(
                        service_,
                        std::move(frontend_socket),
                        backend_pool_.next_backend(),
                        8192)->start_session();
                break;
            }
            case mode::http: {
                std::make_shared<http_session>(
                        service_,
                        std::move(frontend_socket),
                        backend_pool_.next_backend())->start_session();
                break;
            }
            case mode::ssl: {
                std::make_shared<ssl_session>(
                        service_,
                        backend_pool_.next_backend())->start_session();
                break;
            }
        }
    }
    else {
        // TODO: log error
    }
    start_accept();
}

