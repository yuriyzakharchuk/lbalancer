#ifndef LOAD_BALANCER_SESSION_HPP
#define LOAD_BALANCER_SESSION_HPP


#include <boost/asio.hpp>

#include "session_handler.hpp"
#include "../backend/backend.hpp"


namespace lb::session {

    class session : public std::enable_shared_from_this<session> {
    public:
        using backend_t     = lb::backend::backend;
        using socket_t      = boost::asio::ip::tcp::socket;
        using resolver_t    = boost::asio::ip::tcp::resolver;
        using buffer_t      = boost::asio::streambuf;

        explicit session(boost::asio::io_service &,
                         backend_t &,
                         session_handler &);

        socket_t &
        get_socket();

        void
        start_session();

    private:
        boost::asio::io_service &service_;
        backend_t &backend_;

        socket_t frontend_socket_;
        socket_t backend_socket_;

        buffer_t fb_buffer_;
        buffer_t bf_buffer_;

        void
        frontend_read();

        void
        frontend_write();

        void
        backend_read();

        void
        backend_write();
    };
}


#endif //LOAD_BALANCER_SESSION_HPP
