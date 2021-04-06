CMAKE_MINIMUM_REQUIRED(VERSION 3.10)
PROJECT(lbalancer VERSION 0.1 LANGUAGES CXX)

SET(CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_COMPILER clang++)
SET(CMAKE_C_COMPILER clang)
ADD_COMPILE_OPTIONS(
        -Werror
        -Wall
        -Wextra
        -Wpedantic
        -Wcast-align
        -Wcast-qual
        -Wconversion
        -Wctor-dtor-privacy
        -Wenum-compare
        -Wfloat-equal
        -Wnon-virtual-dtor
        -Wold-style-cast
        -Woverloaded-virtual
        -Wredundant-decls
        -Wsign-conversion
        -Wsign-promo
)
if(NOT CMAKE_CXX_EXTENSIONS)
    set(CMAKE_CXX_EXTENSIONS OFF)
endif()


INCLUDE_DIRECTORIES(third_party/certify/include)

FIND_PACKAGE(yaml-cpp REQUIRED)
FIND_PACKAGE(OpenSSL REQUIRED)
FIND_PACKAGE(Boost 1.71 REQUIRED COMPONENTS system thread log)

SET(Boost_USE_STATIC_LIBS OFF)
SET(Boost_USE_MULTITHREAD ON)
SET(BOOST_LIBS ${Boost_SYSTEM_LIBRARY}
               ${Boost_LOG_LIBRARY})


ADD_EXECUTABLE(lbalancer_cli
        src/main.cpp
        src/core/backend/backend_pool.cpp
        src/core/backend/backend.cpp
        src/core/session/tcp/tcp_session.cpp
        src/core/session/http/http_session.cpp
        src/core/workers/service_pool.cpp
        src/core/frontend/frontend.cpp

        src/helpers/configurator/configurator.cpp
        src/os/unix/daemonize.cpp
        src/helpers/logger/basic_logger.cpp
        src/core/session/ssl/ssl_session.cpp)

TARGET_LINK_LIBRARIES(lbalancer_cli PRIVATE ${BOOST_LIBS} yaml-cpp OpenSSL::SSL)
