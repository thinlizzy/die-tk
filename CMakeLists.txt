cmake_minimum_required(VERSION 3.10)
project(die_tk)

set(CMAKE_CXX_STANDARD 17)

include_directories(.)

add_library(die-tk die-tk.h)
target_link_libraries(die-tk die-tk-base die-tk-win)

add_subdirectory(die-tk)
add_subdirectory(die-tk-image)
add_subdirectory(examples/events)
add_subdirectory(examples/two-windows)
#add other examples
