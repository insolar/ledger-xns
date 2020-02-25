cmake_minimum_required(VERSION 3.13.4)
project(ledger-xns-tests VERSION 0.1 LANGUAGES C CXX)


add_definitions(-DOS_IO_SEPROXYHAL=1)
add_definitions(-DBOLOS_SDK=1)
#add_definitions(-DTARGET_NANOS=1)
add_definitions(-DIO_SEPROXYHAL_BUFFER_SIZE_B=300)
add_definitions(-DUSB_SEGMENT_SIZE=64)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_C_STANDARD 11)

set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_BINARY_DIR})

include(cmake/conan.cmake)
include(cmake/collect_sources.cmake)

conan_cmake_install(CONANFILE conanfile.txt -s compiler.libcxx=libstdc++11  CMAKE_TARGETS BUILD missing)

add_library(jsmn src/ledger-xns-app/deps/jsmn/src/jsmn.c)

file(GLOB_RECURSE USER_LIB_SRC
        src/ledger-xns-app/src/lib/parser.c
        src/ledger-xns-app/src/lib/parser_impl.c

        src/ledger-xns-app/src/lib/json/json_parser.c
        src/ledger-xns-app/src/lib/json/tx_parser.c
        src/ledger-xns-app/src/lib/json/tx_display.c
        src/ledger-xns-app/src/lib/json/tx_validate.c
        )

add_library(firmwareapp ${USER_LIB_SRC})
target_include_directories(firmwareapp PUBLIC src/ledger-xns-app/deps/jsmn/src
        src/ledger-xns-app/deps/ledger-zxlib/include
        src/ledger-xns-app/src/lib
        src/ledger-xns-app/src
        nanos-secure-sdk/include
        nanos-secure-sdk/lib_ux/include
        )


find_package(GTest 1.10.0 REQUIRED)
find_package(JsonCpp REQUIRED)
find_package(fmt REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

enable_testing()

CollectSources(TestSources TestHeaders ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/tests)

add_executable(${PROJECT_NAME} ${TestSources})
target_include_directories(${PROJECT_NAME} PUBLIC src/ledger-xns-app/src/lib src/ledger-xns-app/deps/ledger-zxlib/include)
target_link_libraries(${PROJECT_NAME} JsonCpp::JsonCpp fmt::fmt firmwareapp jsmn GTest::GTest)

include(GoogleTest)
gtest_add_tests(TARGET ${PROJECT_NAME}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests
        TEST_SUFFIX .noArgs
        TEST_LIST   noArgsTests
        )
