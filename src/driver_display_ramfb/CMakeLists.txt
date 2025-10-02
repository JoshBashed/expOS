cmake_minimum_required(VERSION 3.20)
project(driver_display_ramfb C)
file(GLOB SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c
)

add_library(driver_display_ramfb STATIC ${SRCS})

# Libraries
target_link_libraries(driver_display_ramfb PUBLIC shared)
target_link_libraries(driver_display_ramfb PUBLIC logging)

target_include_directories(driver_display_ramfb PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
