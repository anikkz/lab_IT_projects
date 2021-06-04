cmake_minimum_required(VERSION 3.2)

project(hl_event_server C CXX)

SET (EXAMPLE_BINARY "hl_labs")
SET (WRITER_BINARY  "event_writer")

find_package(OpenSSL)
find_package(GTest REQUIRED)
find_package(Threads)
find_package(ZLIB)
find_package(Poco REQUIRED COMPONENTS Foundation Util Net XML JSON Crypto NetSSL)
find_package(CppKafka REQUIRED)
find_package(Boost COMPONENTS filesystem system program_options regex REQUIRED)

if(NOT ${Poco_FOUND})
    message(FATAL_ERROR "Poco C++ Libraries not found.")
endif()
include_directories(SYSTEM ${IGNITE_INCLUDE_DIR})
include_directories(${Poco_INCLUDE_DIRS})
include_directories(${GTEST_INCLUDE_DIRS})
include_directories("/usr/include/mysql")
include_directories("/usr/local/include/mysql")
link_directories("/usr/local/lib")
include_directories(${Boost_INCLUDE_DIR})

add_executable(gtests tests/tests.cpp config/config.cpp database/database.cpp database/person.cpp database/cache.cpp)
add_executable(${EXAMPLE_BINARY} main.cpp config/config.cpp database/database.cpp database/person.cpp database/cache.cpp)
add_executable(${WRITER_BINARY} writer_main.cpp config/config.cpp database/database.cpp database/person.cpp database/cache.cpp)

target_include_directories(${EXAMPLE_BINARY} PRIVATE "${CMAKE_BINARY_DIR}")
target_include_directories(${WRITER_BINARY} PRIVATE "${CMAKE_BINARY_DIR}")

target_compile_options(${EXAMPLE_BINARY} PRIVATE -Wall -Wextra -pedantic -Werror )
target_compile_options(${WRITER_BINARY} PRIVATE -Wall -Wextra -pedantic -Werror )

target_link_libraries(${EXAMPLE_BINARY} PRIVATE
        ${CMAKE_THREAD_LIBS_INIT}
        ${Poco_LIBRARIES}
        "ignite-binary"
        "ignite"
        "ignite-common"
        "ignite-thin-client"
        "PocoData"
        "PocoDataMySQL"
        "mysqlclient"
        ZLIB::ZLIB
        "rdkafka++"
        CppKafka::cppkafka)

target_link_libraries(${WRITER_BINARY} PRIVATE
        ${CMAKE_THREAD_LIBS_INIT}
        ${Poco_LIBRARIES}
        "ignite-binary"
        "ignite"
        "ignite-common"
        "ignite-thin-client"
        "PocoData"
        "PocoDataMySQL"
        "mysqlclient"
        ZLIB::ZLIB
        "rdkafka++"
        CppKafka::cppkafka
        ${Boost_LIBRARIES})

target_link_libraries(gtests PRIVATE
        ${GTEST_LIBRARIES}
        ${CMAKE_THREAD_LIBS_INIT}
        ${Poco_LIBRARIES}
        "ignite-binary"
        "ignite"
        "ignite-common"
        "ignite-thin-client"
        "PocoData"
        "PocoDataMySQL"
        "mysqlclient"
        ZLIB::ZLIB
        "rdkafka++"
        CppKafka::cppkafka)


set_target_properties(${EXAMPLE_BINARY} PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(${WRITER_BINARY} PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(${EXAMPLE_BINARY} PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED ON)
set_target_properties(${WRITER_BINARY} PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED ON)
set_target_properties(gtests PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED ON)

install(TARGETS ${EXAMPLE_BINARY} RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
set(CPACK_PACKAGE_CONTACT Dmitriy.Dzyuba@mts.ru)
include(CPack)

enable_testing()
add_test(gtests gtests)

add_executable(gen_data shard_gen_data.cpp)
target_compile_options(gen_data PRIVATE -Wall -Wextra -pedantic -Werror )
set_target_properties(gen_data PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(gen_data PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED ON)
