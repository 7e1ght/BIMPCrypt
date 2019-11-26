cmake_minimum_required(VERSION 3.5)

project(BMPCrypt LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(bmp_crypt STATIC
    bmpcrypt.cpp bmpcrypt.h
    )

add_executable(BMPCrypt main.cpp)

target_link_libraries(BMPCrypt
    bmp_crypt
    )
