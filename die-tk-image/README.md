# die-tk-image - Diego's Graphic Toolkit image utilities
This library uses http://github.com/thinlizzy/libimage in order to load the most popular image formats into die-tk images.

# COMPILE
Sorry! No build system yet, so you need to set your favorite C++ compiler to C++14 and build a library with all files under src folder or just include die-tk-image/src files in the die-tk build. It depends on libimage - http://github.com/thinlizzy/libimage

Windows builds need to include win32 directory as well.

Linux builds need to include linux directory likewise.

# LINK
Applications that use die-tk-image need to link with FreeImage lib.
