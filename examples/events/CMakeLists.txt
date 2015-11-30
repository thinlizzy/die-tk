cmake_minimum_required(VERSION 3.3)
project(events)

set(SOURCE_FILES
    model/Ball.cpp
    model/Ball.h
    model/Painting.cpp
    model/Painting.h
    model/TextLog.cpp
    model/TextLog.h
    view/BallView.cpp
    view/BallView.h
    view/LinesView.cpp
    view/LinesView.h
    view/PaintingView.cpp
    view/PaintingView.h
    main.cpp)

include_directories("../../src")

add_executable(events ${SOURCE_FILES})
set_property(TARGET events PROPERTY CXX_STANDARD 14)

IF(WIN32 AND NOT UNIX)
    FIND_LIBRARY(GDI32_LIB gdi32)
    set(OS_LIBS ${GDI32_LIB})
ELSE(WIN32 AND NOT UNIX)
    FIND_LIBRARY(X11_LIB X11)
    set(OS_LIBS ${X11_LIB})
ENDIF(WIN32 AND NOT UNIX)
target_link_libraries(events die-tk ${OS_LIBS})