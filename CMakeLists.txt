cmake_minimum_required(VERSION 3.20.0)

set(lib joystick_mh)
PROJECT(${lib} C)

set(${lib}_libtype PRIVATE PARENT_SCOPE)

file(GLOB SRCSC "./*.c")
file(GLOB SRCSH "./*.h")
add_library(${lib} ${${lib}_libtype} ${SRCSH} ${SRCSC})
target_link_libraries(${lib} PUBLIC zephyr_interface)
target_link_libraries(${lib} PUBLIC xgpio)
target_link_libraries(${lib} PUBLIC xdts)
target_link_libraries(${lib} PUBLIC xglob)
target_link_libraries(${lib} PUBLIC xlog)
target_include_directories(${lib} PUBLIC ./)
