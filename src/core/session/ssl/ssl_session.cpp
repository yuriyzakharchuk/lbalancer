//
// Created by yuriy on 3/15/21.
//

#include "ssl_session.hpp"


lb::session::ssl_session::ssl_session(boost::asio::io_service &service, backend_t &backend)
        : lb::session::basic_session(service, backend),
          fb_buffer_(),
          bf_buffer_() {
}


void
lb::session::ssl_session::start_session() {

}

