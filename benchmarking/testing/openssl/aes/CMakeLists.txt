cmake_minimum_required(VERSION 3.14)

set(program test_openssl_aes)

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} srcs)
 
add_executable(${program} ${srcs})
target_link_libraries(${program} ${OPENSSL_CRYPTO_LIBRARY})
 
add_custom_command(TARGET ${program}
    POST_BUILD
    COMMAND ./${program}
    COMMENT "run ${program}"
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    DEPENDS ${enclave})