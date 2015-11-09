# die-tk-controls - Diego's Graphic Toolkit controls library
This is a wrapper for basic GUI controls provided by the windowing system. Only win32 bindings are supported for now.

# COMPILE
Sorry! No build system yet, so you need to set your favorite C++ compiler to C++14 and build a library with all files under src directory or just include die-tk-controls/src files in the die-tk build.

Windows builds need to include win32 directory as well.

(TBD) Linux builds need to include linux directory likewise. 

Library clients should include die-tk-controls/die-tk-controls.h, which includes all necessary header files from src subdirectories.

# LINK
In addition to the required die-tk bindings, applications that use die-tk-controls will need to link with comctl32 and comdlg32 (win32). Linux is TBD.

# TEST
The directory examples has some example applications to experiment with die-tk-controls.

# TODO
- add Linux bindings
