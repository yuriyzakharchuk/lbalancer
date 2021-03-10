#include "core/server/server.hpp"

//#include "helpers/configurator.hpp"
#include "os/unix/unix.hpp"

int
main(int argc, char *argv[]) {
    // testing: listening all IPv4 addresses for connection on port 5000
    // TODO: parse settings file
    // TODO: parse args

    // lb::helpers::configurator configurator;
//#ifdef NDEBUG
    //if (lb::os_unix::daemonize()) {

    //}
//#endif //NDEBUG

    lb::server server { "0.0.0.0", "5000", 6 };
    server.run();
    return 0;
}
