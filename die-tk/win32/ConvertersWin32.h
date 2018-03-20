#ifndef CONVERTER_FUNCTIONS_TO_WINDOWS_67EA0445_D4A4_48a1_BB65_24F77640F1AAxx
#define CONVERTER_FUNCTIONS_TO_WINDOWS_67EA0445_D4A4_48a1_BB65_24F77640F1AAxx

#include <windows.h>
#include <string>
#include "../objects/Dimension.h"
#include "../objects/Point.h"
#include "../objects/Rect.h"
#include "../objects/Drawing.h"
#include "../TextParams.h"
#include "../Event.h"
#include "ScopedObjects.h"

namespace tk {

WDims sizeToWDims(SIZE size);
WDims lParamToWDims(LPARAM lParam);
LPARAM WDimsToLParam(WDims dims);
Point lParamToPoint(LPARAM lParam);
Point convertPoint(POINT const & point);
POINT convertPoint(Point const & point);
Rect convertRect(RECT const & rect);
RECT convertRect(Rect const & rect);
RECT convertOpenRect(Rect const & rect);

UserEvent toUserEvent(UINT message, LPARAM lParam);
MouseEvent toMouseEvent(UINT message, WPARAM wParam);

WindowKey fromWindowsKey(WPARAM wParam, LPARAM lParam);
WPARAM toWindowsKey(WindowKey key);

COLORREF colorWin(RGBColor const & color);

UINT convertTextAlign(HTextAlign hta, VTextAlign vta);
int convertPenStyle(PenStyle style);

std::string windowsMessageToString(UINT message);

}

#endif
