# die-tk-image - Diego's Graphic Toolkit image utilities
This library uses github.com/thinlizzy/libimage in order to load the most popular image formats into die-tk images.

# COMPILE
Sorry! No build system yet, so you need to set your favorite C++ compiler to C++14 and build a library with all files under src folder.
Windows builds need to include all win32 folders and exclude all linux folders from the build. 
Linux builds should do it similarly: include linux directories and exclude win32 directories.

# LINK
Applications that use die-tk-image need to link with FreeImage lib.
