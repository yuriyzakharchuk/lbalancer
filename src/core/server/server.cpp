#include "server.hpp"

#include <boost/asio.hpp>
#include <utility>

lb::server::server(const std::string &accept_address,
                   const std::string &port,
                   std::size_t num_threads)
        : service_pool_(num_threads),
          signals_(service_pool_.get_service()),
          acceptor_(service_pool_.get_service()) {

    using boost::asio::ip::tcp;

    tcp::resolver resolver(acceptor_.get_executor());
    tcp::resolver::query query(accept_address, port);
    tcp::endpoint endpoint { *resolver.resolve(query) };

    acceptor_.open(endpoint.protocol());
    acceptor_.set_option(tcp::acceptor::reuse_address(true));
    acceptor_.bind(endpoint);
    acceptor_.listen();

    namespace back = lb::backend;

    // TODO: manage frontends, now just one
    for (int i = 0; i < 1; ++i) {
        // TODO: refactor that shit
        std::vector<std::pair<std::string, std::string>> servers;
        servers.emplace_back(std::pair("127.0.0.1", "8000"));
        //servers.emplace_back(std::pair("192.168.122.97", "9000"));
        //servers.emplace_back(std::pair("192.168.122.227", "9000"));
        //servers.emplace_back(std::pair("192.168.122.17", "9000"));
        //servers.emplace_back(std::pair("91.202.128.71", "80"));

        auto f = lb::frontend::frontend(service_pool_, acceptor_, back::backend_pool {
                back::strategy::round_robin,
                servers,
                service_pool_.get_service() });
        frontend_pool_.push_back(std::move(f));
    }
    start_accept();
}


void
lb::server::start_accept() {
    for (auto &frontend : frontend_pool_) {
        frontend.start_accept();
    }
}


void
lb::server::run() {
    service_pool_.run_all();
}

