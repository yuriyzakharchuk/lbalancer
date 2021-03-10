#ifndef LOAD_BALANCER_TCP_SESSION_HPP
#define LOAD_BALANCER_TCP_SESSION_HPP

#include "../basic_session.hpp"


namespace lb::session {

    class tcp_session : public lb::session::basic_session,
                        public std::enable_shared_from_this<tcp_session> {
    public:
        explicit tcp_session(service_t &, backend_t &);

        void
        start_session() override;

    private:
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


#endif //LOAD_BALANCER_TCP_SESSION_HPP
