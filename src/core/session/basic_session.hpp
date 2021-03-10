//
// Created by yuriy on 3/10/21.
//

#ifndef LOAD_BALANCER_BASIC_SESSION_HPP
#define LOAD_BALANCER_BASIC_SESSION_HPP

#include <boost/asio.hpp>
#include "../backend/backend.hpp"

namespace lb::session {

    class basic_session {

    public:
        using socket_t      = boost::asio::ip::tcp::socket;
        using resolver_t    = boost::asio::ip::tcp::resolver;
        using buffer_t      = boost::asio::streambuf;
        using service_t     = boost::asio::io_service;
        using backend_t     = lb::backend::backend;

        explicit basic_session(boost::asio::io_service & service, backend_t & backend)
            : service_(service),
              backend_(backend),
              frontend_socket_(service),
              backend_socket_(service) {

        }

        socket_t &
        get_socket() {
            return frontend_socket_;
        }

        virtual void
        start_session() = 0;

        virtual
        ~basic_session() {
        }

    protected:
        socket_t frontend_socket_;
        socket_t backend_socket_;

        service_t &service_;
        backend_t &backend_;
    };
}



#endif //LOAD_BALANCER_BASIC_SESSION_HPP
