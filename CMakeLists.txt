cmake_minimum_required(VERSION 3.29)
project(capnp_demo)
set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR}/out)

set(OPENSSL_INCLUDE_DIR  ${PROJECT_SOURCE_DIR}/library/OpenSSL-Win64/include)
set(OPENSSL_ROOT_DIR  ${PROJECT_SOURCE_DIR}/library/OpenSSL-Win64/lib/VC/x64/MTd)

set(ZLIB_INCLUDE_DIR  ${PROJECT_SOURCE_DIR}/library/zlib-1.3.1/include)
set(ZLIB_LIBRARY  ${PROJECT_SOURCE_DIR}/library/zlib-1.3.1/lib/libzlibstatic.a)

add_executable(capnp_demo main.cpp)

add_subdirectory(library/capnproto-c++-1.2.0)
target_link_libraries(capnp_demo CapnProto::capnp CapnProto::kj)

add_subdirectory(capnp)