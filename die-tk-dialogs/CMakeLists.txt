if (WIN32)
    FIND_LIBRARY(COMDLG32_LIB comDlg32 HINTS ${WINDOWS_HINTS})

    add_library(die-tk-dialogs
            Dialogs.h
            SelectFileParams.h
            win32/Dialogs.cpp)

    target_link_libraries(die-tk-dialogs ${COMDLG32_LIB})
else()
    add_library(die-tk-dialogs
            Dialogs.h
            SelectFileParams.h
            linux/Dialogs.cpp
            linux/libsofd.h
            linux/libsofd.c)
endif()
