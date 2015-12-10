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

add_executable(events ${SOURCE_FILES})
set_property(TARGET events PROPERTY CXX_STANDARD 14)

target_link_libraries(events die-tk)