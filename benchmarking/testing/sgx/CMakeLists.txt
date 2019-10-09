cmake_minimum_required(VERSION 3.14)
 
#project(testing)

set(appDir ${PROJECT_SOURCE_DIR}/app)

include_directories(
    ${sgxPath}/include 
    ${PROJECT_BINARY_DIR}/app
    ${appDir}
)

# must
set(rtsLib sgx_urts${enclaveLibSuffix})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${sgxFlags} \
    -fPIC -Wno-attributes -Wnon-virtual-dtor -std=c++14 \
	-DDEBUG -UNDEBUG")

set(appSrcs 
    ${appDir}/errors.cpp 
    ${appDir}/loader.cpp 
)

# following definition to enable benchmarking for Catch2
add_compile_definitions(
    CATCH_CONFIG_ENABLE_BENCHMARKING=1
)

add_subdirectory(aes)
#add_subdirectory(sha256)