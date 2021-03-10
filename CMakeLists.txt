CMAKE_MINIMUM_REQUIRED(VERSION 3.10)
PROJECT(load_balancer)

SET(CMAKE_CXX_STANDARD 17)

# PThread library
SET(THREADS_PREFER_PTHREAD_FLAG ON)
FIND_PACKAGE(Threads REQUIRED)

# Boost.Asio library
SET(Boost_USE_STATIC_LIBS OFF)
SET(Boost_USE_MULTITHREAD ON)
FIND_PACKAGE(Boost 1.71 REQUIRED COMPONENTS system
                                            thread)
if (Boost_FOUND)
    INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})
    LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})
endif (Boost_FOUND)
SET(BOOST_LIBS ${Boost_SYSTEM_LIBRARY}
               ${Boost_THREAD_LIBRARY}
               ${Boost_REGEX_LIBRARY})

# add_definitions(-DBOOST_ASIO_ENABLE_HANDLER_TRACKING)

# Jsoncpp library
FIND_PACKAGE(PkgConfig REQUIRED)
PKG_CHECK_MODULES(JSONCPP jsoncpp)
LINK_LIBRARIES(${JSONCPP_LIBRARIES})

ADD_EXECUTABLE(load_balancer
        src/main.cpp
        src/core/server/server.cpp
        src/core/backend/backend_pool.cpp
        src/core/backend/backend.cpp
        src/core/session/tcp/tcp_session.cpp
        src/core/workers/service_pool.cpp
        src/core/frontend/frontend.cpp

        src/helpers/configurator.cpp
        src/os/unix/daemonize.cpp)

target_link_libraries(load_balancer PRIVATE Threads::Threads
                                            ${BOOST_LIBS}
                                            ${JSONCPP_LIBRARIES})
