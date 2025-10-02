cmake_minimum_required(VERSION 3.20)
project(shared C)
file(GLOB SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c
)

add_library(shared STATIC ${SRCS})
target_include_directories(shared PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
