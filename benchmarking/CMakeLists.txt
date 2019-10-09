cmake_minimum_required(VERSION 3.14)

project(benchmarking)

include(cmake/sgx.cmake)

set(vendorDir ${CMAKE_CURRENT_SOURCE_DIR}/vendor)

#add_subdirectory(app)
add_subdirectory(enclave)

add_subdirectory(testing)