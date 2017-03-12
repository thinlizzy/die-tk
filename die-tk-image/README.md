# die-tk-image - Diego's Graphic Toolkit image utilities
This library uses http://github.com/thinlizzy/libimage in order to load the most popular image formats into die-tk images.

# TODO
Convert this library into a libimage plug-in to be used in die-tk. This will allow supporting other image libraries.

# COMPILE
Use Bazel, CMake or set your favorite C++ compiler to C++14 and build a library with all files under src folder or just include die-tk-image/src files in the die-tk build. It depends on libimage - http://github.com/thinlizzy/libimage

Windows builds need to include win32 directory as well.

Linux builds need to include linux directory likewise.

Client code should include die-tk-image/src/convert.h in order to import images loaded by libimage into die-tk.

# LINK
In addition to the required die-tk bindings, applications that use die-tk-image need to link with FreeImage lib.
