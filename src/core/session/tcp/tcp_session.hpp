#ifndef LOAD_BALANCER_TCP_SESSION_HPP
#define LOAD_BALANCER_TCP_SESSION_HPP

#include <boost/asio.hpp>
#include "../../backend/backend.hpp"


namespace lb::session {
    class tcp_session : public std::enable_shared_from_this<tcp_session> {
    public:
        using socket_t      = boost::asio::ip::tcp::socket;
        using resolver_t    = boost::asio::ip::tcp::resolver;
        using buffer_t      = boost::asio::streambuf;
        using service_t     = boost::asio::io_service;
        using backend_t     = backend::backend;
        using bufflen_t     = std::size_t;


        explicit  tcp_session(service_t&, socket_t&&, backend_t &, bufflen_t);

        tcp_session(tcp_session&&) = delete;
        tcp_session(const tcp_session&) = delete;
        tcp_session& operator=(tcp_session&&) = delete;
        tcp_session& operator=(const tcp_session&) = delete;

        void start_session();

    private:
        socket_t frontend_socket_;
        socket_t backend_socket_;

        service_t &service_;
        backend_t &backend_;

        buffer_t fb_buffer_;
        buffer_t bf_buffer_;
        bufflen_t buffer_len_;

        void frontend_read();
        void frontend_write();
        void backend_read();
        void backend_write();
    };
}


#endif //LOAD_BALANCER_TCP_SESSION_HPP
