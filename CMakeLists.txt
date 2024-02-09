cmake_minimum_required(VERSION 3.28)
project(SiaLibrary VERSION 0.0.3)

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
            -Wstrict-null-sentinel -Wstrict-overflow=5 -Wswitch-default -Wundef -Wno-unused -Og
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

# Sia
add_library(Sia INTERFACE)
target_sources(Sia INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/sia/sia/sia_lib.hpp>
)
target_compile_features(Sia INTERFACE cxx_std_23)
target_include_directories(Sia INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/sia>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)
target_link_libraries(Sia INTERFACE Eigen3::Eigen)

# v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^

# Sia DEV
add_library(Sia_DEV INTERFACE)
target_sources(Sia_DEV INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/sia/dev/network.hpp>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/sia/dev/network.ipp>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/sia/dev/input.ipp>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/sia/dev/general.ipp>
)
target_compile_features(Sia_DEV INTERFACE cxx_std_23)
target_include_directories(Sia_DEV INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)
target_link_libraries(Sia_DEV INTERFACE Eigen3::Eigen)

#=============================================================================

# SiaLibrary LOCAL
add_executable(DEV development/main.cpp)
target_link_libraries(DEV PRIVATE benchmark::benchmark Sia_DEV)

# SiaLibrary TESTING
enable_testing()
include(CTest)

# Test 1
add_executable(testing_1 tests/test_1.cpp)
target_link_libraries(testing_1 PRIVATE Sia_DEV GTest::GTest GTest::Main benchmark::benchmark)
add_test(NAME TEST_1 COMMAND testing_1)

# Test 2
add_executable(testing_2 tests/test_2.cpp)
target_link_libraries(testing_2 PRIVATE Sia_DEV GTest::GTest GTest::Main benchmark::benchmark)
add_test(NAME TEST_2 COMMAND testing_2)

# Test 3
add_executable(testing_3 tests/test_3.cpp)
target_link_libraries(testing_3 PRIVATE Sia_DEV GTest::GTest GTest::Main benchmark::benchmark)
add_test(NAME TEST_3 COMMAND testing_3)

#=============================================================================
# Install

install(TARGETS Sia EXPORT Sia)
install(DIRECTORY sia/ TYPE INCLUDE)
install(EXPORT Sia DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Sia NAMESPACE Sia::)