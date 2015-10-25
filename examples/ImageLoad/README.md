# die-tk Image Load example
Example app that shows how to load images to be used with the TK.

# COMPILE/LINK
- link with die-tk and die-tk-image libs
- die-tk-image needs libimage lib which needs FreeImage lib
- Windows build needs FreeImage.dll and vcomp120.dll in the system path
- Copy/link either FreeImage32.dll or FreeImage64.dll to FreeImage.dll, according to the compiler version used.