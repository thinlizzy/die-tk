FIND_LIBRARY(GDI32_LIB gdi32)
FIND_LIBRARY(USER32_LIB user32)

add_library(die-tk-impl
        components/ImageCanvasWin32.cpp
        components/ImageCanvasWin32.h
        components/ImageWin32.cpp
        components/ImageWin32.h
        components/WindowComponent.h
        controls/ControlMacros.h
        controls/Image.cpp
        controls/ImageWin32.cpp
        controls/ImageWin32.h
        controls/PaintBox.cpp
        controls/PaintBoxWin32.cpp
        controls/PaintBoxWin32.h
        Application.cpp
        CanvasImplWin32.cpp
        CanvasImplWin32.h
        ControlWin32.cpp
        ConvertersWin32.cpp
        ConvertersWin32.h
        die-tk-win32.h
        log.cpp
        NativeControlWin32.cpp
        NativeControlWin32.h
        NativeString.cpp
        ResourceManager.cpp
        ResourceManager.h
        ScopedObjects.h
        SurfaceWin32.cpp
        Window.cpp
        WindowImplWin32.cpp
        WindowImplWin32.h
        )

target_link_libraries(die-tk-impl die-tk-base ${GDI32_LIB} ${USER32_LIB})
