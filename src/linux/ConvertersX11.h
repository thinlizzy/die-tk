#ifndef CONVERTERSX11_H_DIE_TK_2014_SEP_7
#define CONVERTERSX11_H_DIE_TK_2014_SEP_7

#include <string>
#include "../Event.h"
#include "../WindowObjects.h"
#include <X11/Xlib.h>

namespace tk {

std::string xEventToStr(int eventType);

MouseEvent toMouseEvent(XButtonEvent event);

unsigned int toShape(Cursor cursor);

}

#endif
