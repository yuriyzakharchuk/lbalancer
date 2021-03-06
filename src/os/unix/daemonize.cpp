#include "daemonize.hpp"

extern "C" {
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
}

#include <cstdlib>
#include <iostream>

namespace {
[[maybe_unused]] bool already_opened() { return false; }
}  // namespace

bool lb::os_unix::daemonize() {
    umask(0);  // return value: always succeeds
    auto pid{fork()};
    switch (pid) {
        case -1: {
            // TODO: failure handling
            std::exit(1);
        }
        case 0: {
            // in the child
            break;
        }
        default: {
            // in the parent
#ifndef NDEBUG
            std::cout << "Demonized successfully."
                      << " Child's pid: " << pid << std::endl;
#endif  // NDEBUG
            std::exit(0);
        }
    }
    if (setsid() == -1) {
        // TODO: failure handling
    }

    // stdin, stdout, stderr to /dev/null
    auto null_fd{open("/dev/null", O_RDWR)};
    if (null_fd == -1) {
        return false;
    }

    if (dup2(null_fd, STDIN_FILENO) == -1) {
        return false;
    }
    if (dup2(null_fd, STDOUT_FILENO) == -1) {
        return false;
    }
    if (dup2(null_fd, STDERR_FILENO) == -1) {
        return false;
    }

    return true;
}