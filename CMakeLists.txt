cmake_minimum_required(VERSION 3.3)
project(die_tk)

IF(WIN32 AND NOT UNIX)
    set(NON_PORTABLE_SOURCES
        win32/components/ImageWin32.cpp
        win32/components/ImageWin32.h
        win32/components/WindowComponent.h
        win32/Application.cpp
        win32/CanvasImplWin32.cpp
        win32/CanvasImplWin32.h
        win32/ControlWin32.cpp
        win32/ConvertersWin32.cpp
        win32/ConvertersWin32.h
        win32/die-tk-win32.h
        win32/log.cpp
        win32/NativeControlWin32.cpp
        win32/NativeControlWin32.h
        win32/NativeString.cpp
        win32/ResourceManager.cpp
        win32/ResourceManager.h
        win32/ScopedObjects.h
        win32/SurfaceWin32.cpp
        win32/Window.cpp
        win32/WindowImplWin32.cpp
        win32/WindowImplWin32.h)
ELSE(WIN32 AND NOT UNIX)
    set(NON_PORTABLE_SOURCES
        linux/components/ImageX11.cpp
        linux/components/ImageX11.h
        linux/utf8/checked.h
        linux/utf8/core.h
        linux/utf8/unchecked.h
        linux/Application.cpp
        linux/CanvasX11.cpp
        linux/CanvasX11.h
        linux/ClipboardX11.cpp
        linux/ClipboardX11.h
        linux/ControlX11.cpp
        linux/ConvertersX11.cpp
        linux/ConvertersX11.h
        linux/die-tk-linux.h
        linux/log.cpp
        linux/NativeControlImpl.cpp
        linux/NativeControlImpl.h
        linux/NativeString.cpp
        linux/Property.cpp
        linux/Property.h
        linux/ResourceManager.cpp
        linux/ResourceManager.h
        linux/ScopedX11.cpp
        linux/ScopedX11.h
        linux/SurfaceX11.cpp
        linux/utf8.h
        linux/Window.cpp
        linux/WindowImplX11.cpp
        linux/WindowImplX11.h)
ENDIF(WIN32 AND NOT UNIX)

set(SOURCE_FILES
        ${NON_PORTABLE_SOURCES}
        src/components/Image.h
        src/components/ImageParams.h
        src/components/NullImage.cpp
        src/components/NullImage.h
        src/objects/Color.h
        src/objects/Dimension.h
        src/objects/Dimension_impl.h
        src/objects/Drawing.h
        src/objects/Point.h
        src/objects/Rect.h
        src/portable/Control.hpp
        src/portable/Surface.hpp
        src/portable/Window.hpp
        src/util/ClonePtr.h
        src/util/optional.h
        src/util/ScopedHandle.h
        src/Application.h
        src/Callbacks.h
        src/CallbackUtils.h
        src/Canvas.h
        src/Control.h
        src/ControlParams.h
        src/die-tk.h
        src/Event.h
        src/log.h
        src/NativeString.h
        src/NullCanvas.cpp
        src/NullCanvas.h
        src/Surface.cpp
        src/Surface.h
        src/TextParams.h
        src/trace.h
        src/Window.h
        src/WindowObjects.h
        src/WindowParams.h src/util/singleton.h)

add_library(die-tk ${SOURCE_FILES})
set_property(TARGET die-tk PROPERTY CXX_STANDARD 14)

add_subdirectory("examples/events")
