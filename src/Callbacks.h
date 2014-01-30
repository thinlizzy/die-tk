#ifndef CALLBACKS_H_jfd342409gfsdfsj
#define CALLBACKS_H_jfd342409gfsdfsj

#include <functional>
#include "WindowObjects.h"
#include "Event.h"
#include "Canvas.h"

namespace tk {

typedef std::function<bool()> AllowOperation;
typedef std::function<void()> HandleOperation;
typedef std::function<void(Canvas &,Rect)> HandlePaint;
typedef std::function<void(UserEvent)> HandleEvent;
typedef std::function<void(MouseEvent,Point)> HandleMouseButton;
typedef std::function<void(Point)> HandleMouseMove;
typedef std::function<WindowKey(WindowKey)> ProcessKeyEvent;
typedef std::function<char(char)> ProcessKeypress;
typedef std::function<WDims(WDims)> ProcessResize;

}

#endif
