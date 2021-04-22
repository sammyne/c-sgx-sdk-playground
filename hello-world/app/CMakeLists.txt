cmake_minimum_required(VERSION 3.14)

# must
set(rtsLib sgx_urts${enclaveLibSuffix})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${sgxFlags} \
    -fPIC -Wno-attributes -Wnon-virtual-dtor -std=c++14 \
	-DDEBUG -UNDEBUG")

set(bridgeImpl ${CMAKE_CURRENT_BINARY_DIR}/enclave_u.c) 
set(bridge ${CMAKE_CURRENT_BINARY_DIR}/enclave_u.h ${bridgeImpl})

include_directories(
    ${sgxPath}/include
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
)
 
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} srcs)
set(srcs "${srcs};${bridgeImpl}")

add_custom_command(
    OUTPUT ${bridge}
    COMMAND ${sgxEdger8r} --untrusted ${edl} 
       --search-path ${enclavePath} --search-path ${sgxPath}/include
    COMMENT "[GEN] => ${bridge}"
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
 
add_executable(${app} ${srcs})
target_link_libraries(${app} ${sgxFlags} -L${sgxLibPath} ${rtsLib})
 
add_custom_target(run
    ${app} ${PROJECT_BINARY_DIR}/enclave/${enclaveLib}
    DEPENDS ${app} ${enclave}
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)
