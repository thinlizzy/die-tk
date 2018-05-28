add_executable(events
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

target_link_libraries(events die-tk)
