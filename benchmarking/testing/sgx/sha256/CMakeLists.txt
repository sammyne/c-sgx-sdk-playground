cmake_minimum_required(VERSION 3.14)

set(program test_sgx_sha256)

set(bridgeImpl ${CMAKE_CURRENT_BINARY_DIR}/${enclave}_u.c) 
set(bridge ${CMAKE_CURRENT_BINARY_DIR}/${enclave}_u.h ${bridgeImpl})

include_directories(${CMAKE_CURRENT_BINARY_DIR})
 
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} srcs)
set(srcs "${srcs};${appSrcs};${bridgeImpl}")

add_custom_command(
    OUTPUT ${bridge}
    COMMAND ${sgxEdger8r} --untrusted ${edl} 
       --search-path ${enclavePath} --search-path ${sgxPath}/include
    COMMENT "[GEN] => ${bridge}"
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
 
add_executable(${program} ${srcs})
target_link_libraries(${program} ${sgxFlags} -L${sgxLibPath} ${rtsLib})
 
add_custom_command(TARGET ${program}
    POST_BUILD
    COMMAND ./${program}
    COMMENT "run ${program}"
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    DEPENDS ${enclave})