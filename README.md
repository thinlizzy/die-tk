# die-tk - Diego's Graphic Toolkit
This is an Alpha version of a portable GUI TK API. It supports winapi and XLib bindings.

# COMPILE
Sorry! No build system yet, so you need to set your favorite C++ compiler to C++14 and create a library with all files under src folder. 
Library clients should #include <die-tk.h>, which includes all necessary header files from src subdirectories.
Windows builds need to include all win32 folders and exclude all linux folders from the build. 
Linux builds should do it similarly: include linux directories and exclude win32 directories.

# TEST
The directory examples has some example applications to experiment with die-tk.

# TODO
- add a build system, like bazel
- test X11 more and more
- currently some of the Canvas functions are behaving differently in Winapi and XLib. They need to be normalized
- add more example apps, by moving code from die-tk-test2 to them
