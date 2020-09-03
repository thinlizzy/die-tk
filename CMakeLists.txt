cmake_minimum_required(VERSION 3.10)
project(die_tk)

set(CMAKE_CXX_STANDARD 17)

include_directories(.)

add_subdirectory(die-tk)
if (WIN32)
    set(WINDOWS_HINTS "C:/Program Files (x86)/Windows Kits/10/Lib/10.0.18362.0/um/x64")
    add_subdirectory(die-tk/win32)
else()
    add_subdirectory(die-tk/linux)
endif()

add_subdirectory(die-tk-image)
add_subdirectory(die-tk-dialogs)

# examples
add_subdirectory(examples/custom-controls)
add_subdirectory(examples/events)
add_subdirectory(examples/ImageCanvas)
add_subdirectory(examples/ImageScaling)
add_subdirectory(examples/two-windows)
#add other examples
