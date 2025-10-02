cmake_minimum_required(VERSION 3.20)
project(driver_uart C)
file(GLOB SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c
)

add_library(driver_uart STATIC ${SRCS})

# Libraries
target_link_libraries(driver_uart PUBLIC shared)

target_include_directories(driver_uart PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
