add_library(die-tk-image
        convert.h
        win32/convertImage.cpp)
target_link_libraries(die-tk-image die-tk-win)
