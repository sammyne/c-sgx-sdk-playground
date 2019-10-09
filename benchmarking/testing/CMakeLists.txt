cmake_minimum_required(VERSION 3.14)
 
include_directories(${vendorDir}/Catch2/single_include)

add_subdirectory(openssl)
add_subdirectory(sgx)