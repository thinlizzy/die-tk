cmake_minimum_required(VERSION 3.3)
project(die_tk_controls)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

IF(WIN32 AND NOT UNIX)
    set(NON_PORTABLE_SOURCES
        win32/components/ImageList.cpp
        win32/components/ImageListWin32.cpp
        win32/components/ImageListWin32.h
        win32/components/Menu.cpp
        win32/components/MenuWin32.cpp
        win32/components/MenuWin32.h
        win32/components/NativeControlStringList.cpp
        win32/components/NativeControlStringList.h
        win32/controls/Button.cpp
        win32/controls/ButtonBase.cpp
        win32/controls/ButtonBaseWin32.cpp
        win32/controls/ButtonBaseWin32.h
        win32/controls/ButtonWin32.cpp
        win32/controls/ButtonWin32.h
        win32/controls/CheckBox.cpp
        win32/controls/CheckBoxWin32.cpp
        win32/controls/CheckBoxWin32.h
        win32/controls/ComboBox.cpp
        win32/controls/ComboBoxWin32.cpp
        win32/controls/ComboBoxWin32.h
        win32/controls/ControlMacros.h
        win32/controls/Edit.cpp
        win32/controls/EditWin32.cpp
        win32/controls/EditWin32.h
        win32/controls/Image.cpp
        win32/controls/ImageWin32.cpp
        win32/controls/ImageWin32.h
        win32/controls/Label.cpp
        win32/controls/LabelWin32.cpp
        win32/controls/LabelWin32.h
        win32/controls/Memo.cpp
        win32/controls/MemoWin32.cpp
        win32/controls/MemoWin32.h
        win32/controls/PaintBox.cpp
        win32/controls/PaintBoxWin32.cpp
        win32/controls/PaintBoxWin32.h
        win32/controls/TableView.cpp
        win32/controls/TableViewWin32.cpp
        win32/controls/TableViewWin32.h
        win32/controls/TreeView.cpp
        win32/controls/TreeViewWin32.cpp
        win32/controls/TreeViewWin32.h
        win32/CommonControlInitializer.cpp
        win32/CommonControlInitializer.h)
ELSE(WIN32 AND NOT UNIX)
    set(NON_PORTABLE_SOURCES )
ENDIF(WIN32 AND NOT UNIX)

set(SOURCE_FILES
    ${NON_PORTABLE_SOURCES}
    src/components/ImageList.h
    src/components/Menu.h
    src/components/StringList.cpp
    src/components/StringList.h
    src/controls/base/ButtonBase.h
    src/controls/base/ColumnProperties.h
    src/controls/base/ItemProperties.h
    src/controls/Button.h
    src/controls/CheckBox.h
    src/controls/ComboBox.h
    src/controls/Edit.h
    src/controls/Image.h
    src/controls/Label.h
    src/controls/Memo.h
    src/controls/PaintBox.h
    src/controls/TableView.h
    src/controls/TreeView.h
    src/die-tk-controls.h)

add_library(die_tk_controls ${SOURCE_FILES})