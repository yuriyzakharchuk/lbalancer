#include "core/server/server.hpp"

#include "os/unix/unix.hpp"

#include "helpers/logger.hpp"
#include "helpers/configurator.hpp"

int
main(int argc, char *argv[]) {
    lb::helpers::configurator configurator(argc, argv);
    lb::log_init();

#undef __linux__
#ifdef __linux__
    if (!lb::os_unix::daemonize()) {
        lb::logger::log_error("Unable to daemonize application");
        std::exit(1);
    }
#endif

    lb::server server { configurator };
    server.run();
    return 0;
}
