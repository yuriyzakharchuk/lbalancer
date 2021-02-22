cmake_minimum_required(VERSION 3.10)
project(load_balancer)

set(CMAKE_CXX_STANDARD 17)


set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)


set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREAD ON)
find_package(Boost 1.71 REQUIRED COMPONENTS system thread regex)
if (Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    link_directories(${Boost_LIBRARY_DIRS})
endif (Boost_FOUND)

set(BOOST_LIBS ${Boost_SYSTEM_LIBRARY} ${Boost_THREAD_LIBRARY} ${Boost_REGEX_LIBRARY})

# add_definitions(-DBOOST_ASIO_ENABLE_HANDLER_TRACKING)

add_executable(load_balancer
        src/main.cpp
        src/server/server.cpp
        src/backend/backend_pool.cpp
        src/backend/backend.cpp
        src/session/session.cpp
        src/session/session_handler.cpp
        src/workers/service_pool.cpp
        src/frontend/frontend.cpp
        src/helpers/configurator.cpp)

target_link_libraries(load_balancer PRIVATE Threads::Threads ${BOOST_LIBS} config++)
