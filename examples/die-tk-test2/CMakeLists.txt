cmake_minimum_required(VERSION 3.3)
project(test2)

set(SOURCE_FILES
    die-tk-test2.cpp)

add_executable(test2 ${SOURCE_FILES})
set_property(TARGET test2 PROPERTY CXX_STANDARD 14)

target_link_libraries(test2 die-tk)