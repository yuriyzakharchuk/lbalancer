//
// Created by yuriy on 3/15/21.
//

#ifndef LOAD_BALANCER_SSL_SESSION_HPP
#define LOAD_BALANCER_SSL_SESSION_HPP

#include <boost/asio.hpp>
#include "../../backend/backend.hpp"


namespace lb::session {
    class ssl_session : public std::enable_shared_from_this<ssl_session> {
    public:
        using socket_t      = boost::asio::ip::tcp::socket;
        using resolver_t    = boost::asio::ip::tcp::resolver;
        using buffer_t      = boost::asio::streambuf;
        using service_t     = boost::asio::io_service;
        using backend_t     = backend::backend;
        using bufflen_t     = std::size_t;

        explicit ssl_session(service_t &, backend_t &);

        void
        start_session();

    private:

    };

}

#endif //LOAD_BALANCER_SSL_SESSION_HPP
