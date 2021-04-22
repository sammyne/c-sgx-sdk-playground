cmake_minimum_required(VERSION 3.14)

project(hello-world)

set(app app)
include(cmake/sgx.cmake)

set(enclave enclave)
set(enclaveLib ${enclave}.signed.so)
set(enclavePath ${CMAKE_CURRENT_SOURCE_DIR}/enclave)
set(edl enclave.edl)

set(key ${enclavePath}/isv.pem)
set(config ${enclavePath}/enclave.config.xml)

add_subdirectory(app)
add_subdirectory(enclave)