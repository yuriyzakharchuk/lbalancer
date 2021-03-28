//
// Created by yuriy on 3/10/21.
//

#include "basic_logger.hpp"
#include "../configurator/configurator.hpp"

#include <string>
#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/file.hpp>

void
lb::log_init() {
    namespace log      = boost::log;
    namespace keywords = boost::log::keywords;

    const std::string file_name { lb::helpers::configurator::default_config_path };
    log::add_file_log(keywords::file_name = file_name,
                      keywords::target_file_name = file_name,
                      keywords::open_mode = std::ios_base::in );

    // log::core::get()->set_filter (log::trivial::severity >= log::trivial::info);

}


void
lb::console_write(const std::string& message) {
    BOOST_LOG_TRIVIAL(info) << message << '\n';
}

void
lb::log_error(const std::string &message) {

}

void
lb::log_info(const std::string &message) {

}

void
lb::log_trace(const std::string &message) {

}
