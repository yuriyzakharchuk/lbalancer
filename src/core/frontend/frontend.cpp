#include "frontend.hpp"


lb::frontend::frontend::frontend(workers::service_pool &service_pool,
                                 boost::asio::ip::tcp::acceptor &acceptor,
                                 lb::backend::backend_pool backend_pool)
        : service_pool_(service_pool), acceptor_(acceptor),
          backend_pool_(std::move(backend_pool)), new_session_(),
          session_handler_() {
}


void
lb::frontend::frontend::start_accept() {
    new_session_.reset(new lb::session::session {
            service_pool_.get_service(),
            backend_pool_.next_backend(),
            session_handler_ });
    acceptor_.async_accept(new_session_->get_socket(), [this](auto &&error) {
        handle_accept(error);
    });
}


void
lb::frontend::frontend::handle_accept(const boost::system::error_code &error) {
    if (!error) {
        new_session_->start_session();
    }
    start_accept();
}

