#include "../Surface.h"

namespace tk {
    
Rect Surface::rect() const
{
    return impl->rect();
}

Surface & Surface::setPos(Point pos)
{
    impl->setPos(pos);
    return *this;
}

Surface & Surface::setDims(WDims dims)
{
    impl->setDims(dims);
    return *this;
}

Surface & Surface::setRect(Rect rect)
{
    impl->setRect(rect);
    return *this;
}

void Surface::bringToFront()
{
    impl->bringToFront();
}

void Surface::sendToBack()
{
    impl->sendToBack();
}
    
bool Surface::enabled() const
{
    return impl->enabled();
}

void Surface::enable()
{
    impl->enable();
}

void Surface::disable()
{
    impl->disable();
}

bool Surface::visible() const
{
    return impl->visible();
}

void Surface::show()
{
    impl->show();
}

void Surface::hide()
{
    impl->hide();
}

Canvas & Surface::canvas()
{
    return impl->canvas();
}

void Surface::repaint()
{
    impl->repaint();
}

void Surface::setCursor(Cursor cursor)
{
    impl->setCursor(cursor);
}

void Surface::setBackground(RGBColor const & color)
{
    impl->setBackground(color);
}

Point Surface::screenToClient(Point const & point) const
{
    return impl->screenToClient(point);
}

Surface::operator bool() const
{
    return impl.operator bool();
}

NativeString Surface::getText() const
{
    return impl->getText();
}

Surface & Surface::setText(NativeString const & text)
{
    impl->setText(text);
    return *this;
}

ClipboardType Surface::copyToClipboard() const
{
    return impl->copyToClipboard();
}

HandleMouseButton Surface::onMouseDown(HandleMouseButton callback)
{
    return impl->onMouseDown(callback);
}

HandleMouseButton Surface::onMouseUp(HandleMouseButton callback)
{
    return impl->onMouseUp(callback);
}

HandleMouseMove Surface::onMouseEnter(HandleMouseMove callback)
{
    return impl->onMouseEnter(callback);
}

HandleMouseMove Surface::onMouseOver(HandleMouseMove callback)
{
    return impl->onMouseOver(callback);
}

HandleMouseMove Surface::onMouseLeave(HandleMouseMove callback)
{
    return impl->onMouseLeave(callback);
}

ProcessKeyEvent Surface::onKeyDown(ProcessKeyEvent callback)
{
    return impl->onKeyDown(callback);
}

ProcessKeyEvent Surface::onKeyUp(ProcessKeyEvent callback)
{
    return impl->onKeyUp(callback);
}

ProcessKeypress Surface::onKeypress(ProcessKeypress callback)
{
    return impl->onKeypress(callback);
}

HandlePaint Surface::onPaint(HandlePaint callback)
{
    return impl->onPaint(callback);
}

}