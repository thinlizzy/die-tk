cmake_minimum_required(VERSION 3.3)
project(die-tk-image)
include(ExternalProject)

IF(WIN32 AND NOT UNIX)
    set(NON_PORTABLE_SOURCES
        win32/convertImage.cpp)
ELSE(WIN32 AND NOT UNIX)
    set(NON_PORTABLE_SOURCES
        linux/convertImage.cpp)
ENDIF(WIN32 AND NOT UNIX)

set(SOURCE_FILES
    ${NON_PORTABLE_SOURCES}
    src/convert.h)

add_library(die-tk-image ${SOURCE_FILES})
set_property(TARGET die-tk-image PROPERTY CXX_STANDARD 14)
target_include_directories(die-tk-image PUBLIC ${PROJECT_SOURCE_DIR}/src)

ExternalProject_Add(libimage_project
    GIT_REPOSITORY https://github.com/thinlizzy/libimage
    INSTALL_DIR libimage
)
add_library(libimage STATIC IMPORTED)

target_link_libraries(die-tk-image die-tk libimage)
