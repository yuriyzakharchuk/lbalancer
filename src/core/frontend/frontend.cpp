#include "frontend.hpp"


lb::frontend::frontend::frontend(const std::string &accept_address,
                                 const std::string &port,
                                 workers::service_pool &service_pool,
                                 lb::backend::backend_pool backend_pool,
                                 lb::session::mode mode)
        : service_pool_(service_pool), acceptor_(service_pool.get_service()),
          backend_pool_(std::move(backend_pool)), session_(nullptr),
          mode_(mode){

    using boost::asio::ip::tcp;

    tcp::resolver resolver(acceptor_.get_executor());
    tcp::resolver::query query(accept_address, port);
    tcp::endpoint endpoint { *resolver.resolve(query) };

    acceptor_.open(endpoint.protocol());
    acceptor_.set_option(tcp::acceptor::reuse_address(true));
    acceptor_.bind(endpoint);
    acceptor_.listen();
}


void
lb::frontend::frontend::start_accept() {
    // TODO: protocol config handling
    switch (mode_) {
        case lb::session::mode::tcp: {
            session_.reset(new lb::session::tcp_session {
                    service_pool_.get_service(),
                    backend_pool_.next_backend(),
                    8192
            });
            break;
        }
        case lb::session::mode::http: {
            session_.reset(new lb::session::http_session {
                    service_pool_.get_service(),
                    backend_pool_.next_backend()
            });
            break;
        }
        case lb::session::mode::ssl: {
            session_.reset(new lb::session::ssl_session {
                    service_pool_.get_service(),
                    backend_pool_.next_backend()
            });
            break;
        }
    }

    acceptor_.async_accept(session_->get_socket(), [this](auto &&error) {
        handle_accept(error);
    });
}


void
lb::frontend::frontend::handle_accept(const boost::system::error_code &error) {
    if (!error) {
        session_->start_session();
    }
    start_accept();
}

