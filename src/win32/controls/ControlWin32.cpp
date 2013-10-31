#include "NativeControlWin32.h"
#include "../../Control.h"
#include "../../Window.h"
#include "../../WindowRef.h"
#include "../ResourceManager.h"

namespace tk {
    
Rect Control::rect() const
{
    return impl->rect();
}

Control & Control::setPos(Point pos)
{
    impl->setPos(pos);
    return *this;
}

Control & Control::setDims(WDims dims)
{
    impl->setDims(dims);
    return *this;
}

Control & Control::setRect(Rect rect)
{
    impl->setRect(rect);
    return *this;
}

void Control::bringToFront()
{
    impl->bringToFront();
}

void Control::sendToBack()
{
    impl->sendToBack();
}
    
bool Control::enabled() const
{
    return impl->enabled();
}

void Control::enable()
{
    impl->enable();
}

void Control::disable()
{
    impl->disable();
}

bool Control::visible() const
{
    return impl->visible();
}

void Control::show()
{
    impl->show();
}

void Control::hide()
{
    impl->hide();
}

Canvas & Control::canvas()
{
    return impl->canvas();
}

void Control::repaint()
{
    impl->repaint();
}

void Control::setCursor(Cursor cursor)
{
    impl->setCursor(cursor);
}

void Control::setBackground(RGBColor const & color)
{
    impl->setBackground(color);
}

Point Control::screenToClient(Point const & point) const
{
    return impl->screenToClient(point);
}

Control::operator bool() const
{
    return impl.operator bool();
}

die::NativeString Control::getText() const
{
    return impl->getText();
}

Control & Control::setText(die::NativeString const & text)
{
    impl->setText(text);
    return *this;
}

WindowRef Control::getParent() const
{
    return WindowRef(resourceManager.findWindow(impl->getParentHwnd()));
}

HandleMouseEvent Control::onMouse(HandleMouseEvent callback)
{
    return impl->onMouse(callback);
}

ProcessKeyEvent Control::onKey(ProcessKeyEvent callback)
{
    return impl->onKey(callback);
}

ProcessKeypress Control::onKeypress(ProcessKeypress callback)
{
    return impl->onKeypress(callback);
}

HandlePaint Control::onPaint(HandlePaint callback)
{
    return impl->onPaint(callback);
}

}