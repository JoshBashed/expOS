cmake_minimum_required(VERSION 3.20)
project(logging C)
file(GLOB SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c
)

add_library(logging STATIC ${SRCS})

# Libraries
target_link_libraries(logging PUBLIC shared)

target_include_directories(logging PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
