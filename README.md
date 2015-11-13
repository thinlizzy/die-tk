# die-tk - Diego's Graphic Toolkit
This is an Alpha version of a portable GUI TK API. It supports WinApi and XLib bindings.

# COMPILE
I am experimenting with CMake now. Please let me know if it works for you.

For the ones that don't want to use CMake, you need to set your favorite C++ compiler to C++14 and build a library with all files under src directory.
Windows builds need to include win32 directory as well.
Linux builds need to include linux directory likewise.

Library clients should include die-tk.h, which includes all necessary header files from src subdirectories.

The die-tk-image directory is optional. It can also be included in the build when support for loading image files is needed. It depends on http://github.com/thinlizzy/libimage

The die-tk-controls directory is optional as well. It can be included in the build if the application needs basic GUI controls.

# LINK
Applications that use die-tk will need to link with gdi32 (win32) or X11 (linux) libs.

If die-tk-image is included, then applications will need to link with libimage and FreeImage lib.

If die-tk-controls is included, then applications will need to link with comdlg32 (win32). Linux is TBD

# TEST
The directory examples has some example applications to experiment with die-tk.

# TODO
- experiment with Bazel
- add CMakeLists.txt to the example directories and to die-tk-image and die-tk-controls bindings
- test X11 more and more. I think the image routines can be greatly improved
- currently some of the Canvas functions are behaving differently in win32 and X11. They need to be normalized
- add more example apps
