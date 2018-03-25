#ifndef CONVERTERSX11_H_DIE_TK_2014_SEP_7
#define CONVERTERSX11_H_DIE_TK_2014_SEP_7

#include <string>
#include <X11/Xlib.h>
#include "../Event.h"
#include "../WindowObjects.h"
#include "../objects/Color.h"

namespace tk {

std::string xEventToStr(int eventType);

MouseEvent toMouseEvent(XButtonEvent event);

unsigned int toShape(Cursor cursor);

unsigned long rgb32(RGBColor const & color);

WindowKey fromKeySym(KeySym keySym);

unsigned int toKeyCode(WindowKey key);

}

#endif
