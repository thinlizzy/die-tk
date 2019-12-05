FIND_LIBRARY(X11_LIB x11)

add_library(die-tk-impl
        Application.cpp
        CanvasX11.cpp
        CanvasX11.h
        ClipboardX11.cpp
        ClipboardX11.h
        ControlX11.cpp
        ConvertersX11.cpp
        ConvertersX11.h
        die-tk-linux.h
        log.cpp
        NativeControlImpl.cpp
        NativeControlImpl.h
        NativeString.cpp
        Property.cpp
        Property.h
        ResourceManager.cpp
        ResourceManager.h
        ScopedX11.cpp
        ScopedX11.h
        SurfaceX11.cpp
        utf8.h
        Window.cpp
        WindowImplX11.cpp
        WindowImplX11.h
        )

target_link_libraries(die-tk-impl ${X11_LIB})
