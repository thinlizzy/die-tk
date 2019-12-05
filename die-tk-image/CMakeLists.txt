if (WIN32)
    add_library(die-tk-image
            convert.h
            win32/convertImage.cpp)
else()
    add_library(die-tk-image
            convert.h
            linux/convertImage.cpp)
endif()
