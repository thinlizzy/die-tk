#ifndef CONVERTER_FUNCTIONS_TO_WINDOWS_67EA0445_D4A4_48a1_BB65_24F77640F1AAxx
#define CONVERTER_FUNCTIONS_TO_WINDOWS_67EA0445_D4A4_48a1_BB65_24F77640F1AAxx

#include <windows.h>
#include "../WindowObjects.h"
#include "../Event.h"
#include "ScopedObjects.h"
#include <string>
#include <vector>

namespace tk {

Rect convertRect(RECT const & rect);
WDims lParamToWDims(LPARAM lParam);
LPARAM WDimsToLParam(WDims dims);
Point lParamToPoint(LPARAM lParam);
Point convertPoint(POINT const & point);
POINT convertPoint(Point const & point);

UserEvent toUserEvent(UINT message, LPARAM lParam);
MouseEvent toMouseEvent(UINT message, WPARAM wParam);
KeyEvent toKeyEvent(UINT message, WPARAM wParam);

WindowKey fromWindowsKey(WPARAM wParam);
WPARAM toWindowsKey(WindowKey key);

COLORREF colorWin(RGBColor const & color);

std::string windowsMessageToString(UINT message);
std::string errorMessage(DWORD error);

struct NativeBitmap {
    BITMAPINFO info;
	std::vector<unsigned char> imageBuffer;
};

NativeBitmap convertRawImage(unsigned char const buffer[], ImageType type, WDims dim);

scoped::Bitmap ihToBitmap(ImageHolder ih);

}

#endif
