cmake_minimum_required(VERSION 3.28)
project(CoesiusLibrary VERSION 0.0.3)

#=============================================================================
# Local settings

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if (NOT MSVC)
    if(CMAKE_BUILD_TYPE MATCHES Debug)
        add_compile_options(
            -Wall -Wextra -pedantic -Wcast-align -Wcast-qual -Wctor-dtor-privacy -Wdisabled-optimization
            -Wformat=2 -Winit-self -Wlogical-op -Wmissing-declarations -Wmissing-include-dirs -Wnoexcept
            -Wold-style-cast -Woverloaded-virtual -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo
            -Wstrict-null-sentinel -Wstrict-overflow=5 -Wswitch-default -Wundef -Wno-unused -Og -g3
        )
    endif()
    if(CMAKE_BUILD_TYPE MATCHES Release)
        add_compile_options(
            -O3 -march=native
        )
    endif()
endif()

#=============================================================================
# Dependencies

include(GNUInstallDirs)
find_package(benchmark REQUIRED)
find_package(Eigen3 REQUIRED)
find_package(GTest REQUIRED)

#=============================================================================
# Targets

# Coesius
add_library(Coesius INTERFACE)
target_sources(Coesius INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/coesius/coesius/coesius_lib.hpp>
)
target_compile_features(Coesius INTERFACE cxx_std_23)
target_include_directories(Coesius INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/coesius>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)
target_link_libraries(Coesius INTERFACE Eigen3::Eigen)

# v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^

# Coesius DEV
add_library(Coesius_DEV INTERFACE)
target_sources(Coesius_DEV INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/coesius/dev/network.hpp>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/coesius/dev/network.ipp>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/coesius/dev/input.ipp>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/coesius/dev/general.ipp>
)
target_compile_features(Coesius_DEV INTERFACE cxx_std_23)
target_include_directories(Coesius_DEV INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)
target_link_libraries(Coesius_DEV INTERFACE Eigen3::Eigen)

#=============================================================================

# CoesiusLibrary LOCAL
add_executable(DEV dev/main.cpp)
target_link_libraries(DEV PRIVATE benchmark::benchmark Coesius_DEV)
target_link_options(DEV PRIVATE "-lasan" "-lubsan")

# CoesiusLibrary TESTING
enable_testing()
include(CTest)

# Test 1
add_executable(testing_1 tests/test_1.cpp)
target_link_options(testing_1 PRIVATE "-lasan" "-lubsan")
target_link_libraries(testing_1 PRIVATE Coesius_DEV GTest::GTest GTest::Main benchmark::benchmark)
add_test(NAME TEST_1 COMMAND testing_1)

# Test 2
add_executable(testing_2 tests/test_2.cpp)
target_link_options(testing_2 PRIVATE "-lasan" "-lubsan")
target_link_libraries(testing_2 PRIVATE Coesius_DEV GTest::GTest GTest::Main benchmark::benchmark)
add_test(NAME TEST_2 COMMAND testing_2)

# Test 3
add_executable(testing_3 tests/test_3.cpp)
target_link_options(testing_3 PRIVATE "-lasan" "-lubsan")
target_link_libraries(testing_3 PRIVATE Coesius_DEV GTest::GTest GTest::Main benchmark::benchmark)
add_test(NAME TEST_3 COMMAND testing_3)

#=============================================================================
# Install

install(TARGETS Coesius EXPORT Coesius)
install(DIRECTORY coesius/ TYPE INCLUDE)
install(EXPORT Coesius DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/coesius NAMESPACE coesius::)