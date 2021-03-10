#ifndef LOAD_BALANCER_HTTP_SESSION_HPP
#define LOAD_BALANCER_HTTP_SESSION_HPP

#include "../basic_session.hpp"


namespace lb::session {

    class http_session : public lb::session::basic_session,
                         public std::enable_shared_from_this<http_session> {
    public:
        explicit http_session(service_t &, backend_t &);

        void
        start_session() override;

    private:
        buffer_t fb_buffer_;
        buffer_t bf_buffer_;
    };
}


#endif //LOAD_BALANCER_HTTP_SESSION_HPP
