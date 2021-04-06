#include "core/server/server.hpp"
#include "helpers/configurator/configurator.hpp"
#include "helpers/logger/logger.hpp"
#include "os/unix/unix.hpp"

int main(int argc, char *argv[]) {
    lb::helpers::configurator configurator(argc, argv);
    lb::log_init();

#undef __linux__
#ifdef __linux__
    if (!lb::os_unix::daemonize()) {
        lb::log_error("Unable to daemonize application");
        std::exit(EXIT_FAILURE);
    }
#endif

    lb::server server{configurator};
    server.run();
    return EXIT_SUCCESS;
}
