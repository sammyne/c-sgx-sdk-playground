cmake_minimum_required(VERSION 3.14)

project(c-sgx-sdk-playground)

set(vendorDir ${CMAKE_CURRENT_SOURCE_DIR}/vendor)

include(cmake/sgx.cmake)

add_subdirectory(benchmarking-read-write)