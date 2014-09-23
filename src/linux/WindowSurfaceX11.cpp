#include "WindowSurfaceX11.h"
#include "ResourceManager.h"
#include "ConvertersX11.h"

namespace tk {

ControlCallbackMap<HandlePaint> cbPaint;
ControlCallbackMap<HandleMouseButton> cbMouseDown,cbMouseUp;
ControlCallbackMap<HandleMouseMove> cbMouseEnter,cbMouseOver,cbMouseLeave;
ControlCallbackMap<ProcessKeyEvent> cbKeyDown,cbKeyUp;
ControlCallbackMap<ProcessKeypress> cbKeypress;

NativeControlImpl::~NativeControlImpl()
{
}

void NativeControlImpl::setPos(Point pos)
{
	XWindowChanges changes;
	changes.x = pos.x;
	changes.y = pos.y;
	XConfigureWindow(resourceManager.dpy, windowId, CWX | CWY, &changes);
}

void NativeControlImpl::setDims(WDims dims)
{
	XResizeWindow(resourceManager.dpy, windowId,dims.width,dims.height);
}

void NativeControlImpl::setRect(Rect rect)
{
	XWindowChanges changes;
	changes.x = rect.left;
	changes.y = rect.top;
	changes.width = rect.width();
	changes.height = rect.height();
	XConfigureWindow(resourceManager.dpy, windowId, CWX | CWY | CWWidth | CWHeight, &changes);
}

Rect NativeControlImpl::rect() const
{
	XWindowAttributes attrs;
	XGetWindowAttributes(resourceManager.dpy, windowId, &attrs);
	return Rect::closed(Point(attrs.x,attrs.y),WDims(attrs.width,attrs.height));
}

void NativeControlImpl::show()
{
	XMapWindow(resourceManager.dpy, windowId);
}

void NativeControlImpl::hide()
{
	XUnmapWindow(resourceManager.dpy, windowId);
}

bool NativeControlImpl::visible() const
{
	XWindowAttributes attrs;
	XGetWindowAttributes(resourceManager.dpy, windowId, &attrs);
	return attrs.map_state != IsUnmapped ;
}

void NativeControlImpl::bringToFront()
{
	throw "bringToFront not implemented";
}

void NativeControlImpl::sendToBack()
{
	throw "sendToBack not implemented";
}

bool NativeControlImpl::enabled() const
{
	throw "enabled not implemented";
}

void NativeControlImpl::enable()
{
	throw "enable not implemented";
}

void NativeControlImpl::disable()
{
	throw "disable not implemented";
}

die::NativeString NativeControlImpl::getText() const
{
	throw "getText not implemented";
}

void NativeControlImpl::setText(const die::NativeString& text)
{
	throw "setText not implemented";
}

ClipboardType NativeControlImpl::copyToClipboard() const
{
	throw "copyToClipboard not implemented";
}

Canvas & NativeControlImpl::canvas()
{
	throw "canvas not implemented";
}

void NativeControlImpl::repaint()
{
	throw "repaint not implemented";
}

void NativeControlImpl::setCursor(Cursor cursor)
{
	throw "setCursor not implemented";
}

void NativeControlImpl::setBackground(const RGBColor& color)
{
	throw "setBackground not implemented";
}

Point NativeControlImpl::screenToClient(const Point& point) const
{
	throw "screenToClient not implemented";
}

::Window NativeControlImpl::getParentHandle() const
{
	throw "getParentHandle not implemented";
}

ControlParams NativeControlImpl::getControlData() const
{
	throw "getControlData not implemented";
}

// callbacks & messages

HandleMouseButton NativeControlImpl::onMouseDown(HandleMouseButton callback)
{
    return setCallback(this,cbMouseDown,callback);
}

HandleMouseButton NativeControlImpl::onMouseUp(HandleMouseButton callback)
{
    return setCallback(this,cbMouseUp,callback);
}

HandleMouseMove NativeControlImpl::onMouseEnter(HandleMouseMove callback)
{
    return setCallback(this,cbMouseEnter,callback);
}

HandleMouseMove NativeControlImpl::onMouseOver(HandleMouseMove callback)
{
    return setCallback(this,cbMouseOver,callback);
}

HandleMouseMove NativeControlImpl::onMouseLeave(HandleMouseMove callback)
{
    return setCallback(this,cbMouseLeave,callback);
}

ProcessKeyEvent NativeControlImpl::onKeyDown(ProcessKeyEvent callback)
{
    return setCallback(this,cbKeyDown,callback);
}

ProcessKeyEvent NativeControlImpl::onKeyUp(ProcessKeyEvent callback)
{
    return setCallback(this,cbKeyUp,callback);
}

HandlePaint NativeControlImpl::onPaint(HandlePaint callback)
{
    return setCallback(this,cbPaint,callback);
}

ProcessKeypress NativeControlImpl::onKeypress(ProcessKeypress callback)
{
	return setCallback(this,cbKeypress,callback);
}

void NativeControlImpl::processMessage(XEvent & e)
{
	//log::info("window ",e.xany.window," got event ",xEventToStr(e.type));

	switch(e.type) {
	case ButtonPress:
		auto & data = e.xbutton;
		// TODO needs to fill controlPressed and shiftPressed in the mouse event (or remove it)
		executeCallback(this, cbMouseDown, toMouseEvent(data), Point(data.x,data.y));
		break;
	}
}

}

