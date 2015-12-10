cmake_minimum_required(VERSION 3.3)
project(clipboard)

set(SOURCE_FILES
    clipboard.cpp)

add_executable(clipboard ${SOURCE_FILES})
set_property(TARGET clipboard PROPERTY CXX_STANDARD 14)

#target_link_libraries(clipboard die-tk ${OS_LIBS})
target_link_libraries(clipboard die-tk)
