#include "server/server.hpp"


int
main(int argc, char *argv[]) {
    // testing: listening all IPv4 addresses for connection on port 5000
    // TODO: make daemon
    // TODO: parse settings file
    lb::server server { "0.0.0.0", "5000", 6 };
    server.run();
    return 0;
}