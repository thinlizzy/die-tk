# die-tk - Diego's Graphic Toolkit
This is an Alpha version of a portable GUI TK API. It supports WinApi and XLib bindings.

# COMPILE
Sorry! No build system yet, so you need to set your favorite C++ compiler to C++14 and build a library with all files under src directory.

Windows builds need to include win32 directory as well.

Linux builds need to include linux directory likewise.

Library clients should #include <die-tk.h>, which includes all necessary header files from src subdirectories.

The die-tk-image directory is optional and it can be compiled separately as a library that depends on die-tk and libimage. libimage can be found at http://github.com/thinlizzy/libimage

# LINK
Applications that use die-tk will need to link with gdi32 (win32) or X11 (linux) libs.

Applications that use die-tk-image need to link with FreeImage lib.

# TEST
The directory examples has some example applications to experiment with die-tk.

# TODO
- add a build system, like Bazel or CMake
- test X11 more and more. I think the image routines can be greatly improved
- currently some of the Canvas functions are behaving differently in WinApi and X11. They need to be normalized
- add more example apps
