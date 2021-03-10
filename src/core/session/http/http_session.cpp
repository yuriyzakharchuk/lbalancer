#include "http_session.hpp"


lb::session::http_session::http_session(boost::asio::io_service &service, backend_t &backend)
    : lb::session::basic_session(service, backend),
      fb_buffer_(),
      bf_buffer_() {
}


void
lb::session::http_session::start_session() {

}

