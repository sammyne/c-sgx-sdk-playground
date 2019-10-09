cmake_minimum_required(VERSION 3.14)

project(benchmarking-read-write)

set(enclave ${PROJECT_NAME}-enclave)
set(enclaveLib ${enclave}.signed.so)
set(enclavePath ${CMAKE_CURRENT_SOURCE_DIR}/enclave)
set(edl enclave.edl)

set(key ${enclavePath}/isv.pem)
set(config ${enclavePath}/enclave.config.xml)

set(catch2Path ${vendorDir}/Catch2/single_include)
set(utilsPath ${CMAKE_CURRENT_SOURCE_DIR}/utils)

add_subdirectory(enclave)

add_subdirectory(non-sgx)
add_subdirectory(sgx)