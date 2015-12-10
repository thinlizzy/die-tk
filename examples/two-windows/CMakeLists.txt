cmake_minimum_required(VERSION 3.3)
project(two-windows)

set(SOURCE_FILES
    two-windows.cpp)

add_executable(two-windows ${SOURCE_FILES})
set_property(TARGET two-windows PROPERTY CXX_STANDARD 14)

target_link_libraries(two-windows die-tk)