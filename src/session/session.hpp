#ifndef LOAD_BALANCER_SESSION_HPP
#define LOAD_BALANCER_SESSION_HPP


#include <boost/asio.hpp>

#include "session_handler.hpp"
#include "../backend/backend.hpp"


namespace lb::session {

    class session : public std::enable_shared_from_this<session> {
    public:
        using backend_t = lb::backend::backend;
        using socket_t = boost::asio::ip::tcp::socket;
        using resolver_t = boost::asio::ip::tcp::resolver;

        explicit session(boost::asio::io_service &,
                         backend_t &,
                         session_handler &);

        socket_t &
        get_socket();

        void
        run();

    private:
        boost::asio::io_service &service_;
        backend_t &backend_;

        socket_t frontend_socket_;
        socket_t backend_socket_;

        boost::asio::streambuf frontend_reading_buffer_;
        boost::asio::streambuf backend_reading_buffer_;

        void
        start_session(const boost::system::error_code &,
                      const boost::asio::ip::tcp::endpoint &);

        void
        handle_frontend_reading(const boost::system::error_code &,
                                std::size_t);

        void
        handle_frontend_writing(const boost::system::error_code &,
                                std::size_t);

        void
        handle_backend_reading(const boost::system::error_code &,
                               std::size_t);

        void
        handle_backend_writing(const boost::system::error_code &,
                               std::size_t);
    };
}


#endif //LOAD_BALANCER_SESSION_HPP
