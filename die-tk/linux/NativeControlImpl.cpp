#include "NativeControlImpl.h"
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include "ResourceManager.h"
#include "ConvertersX11.h"
#include "../log.h"

namespace {
	tk::ResourceManagerSingleton resourceManager;

	Window createWindow(Window parentWindowId, tk::ControlParams const & params) {
		Window windowId = resourceManager->createWindow(
			params.start_.x, params.start_.y, params.dims_.width, params.dims_.height,
			parentWindowId);
		// TODO convert params.cursor_, params.scrollbar_
		return windowId;
	}
}

namespace tk {

ControlCallbackMap<HandlePaint> cbPaint;
ControlCallbackMap<HandleMouseButton> cbMouseDown, cbMouseUp;
ControlCallbackMap<HandleMouseMove> cbMouseEnter, cbMouseOver, cbMouseLeave;
ControlCallbackMap<ProcessKeyEvent> cbKeyDown, cbKeyUp;
ControlCallbackMap<ProcessKeypress> cbKeypress;

NativeControlImpl::NativeControlImpl(::Window parentWindowId, ::Window windowId):
	parentWindowId(parentWindowId),
	windowId(windowId),
	windowCanvas(CanvasX11(windowId)) {
}

NativeControlImpl::NativeControlImpl(::Window parentWindowId, ControlParams const & params, long event_mask):
	parentWindowId(parentWindowId),
	windowId(createWindow(parentWindowId, params)),
	windowCanvas(CanvasX11(windowId)) {
	if( params.backgroundColor_ ) {
		setBackground(*params.backgroundColor_);
	}
	XSelectInput(resourceManager->dpy, windowId, event_mask);
	if( params.visible_ ) {
		show();
	}
}

NativeControlImpl::~NativeControlImpl() {
}

void NativeControlImpl::setPos(Point pos) {
	// TODO adjust the pos according to the window borders
	XWindowChanges changes;
	changes.x = pos.x;
	changes.y = pos.y;
	XConfigureWindow(resourceManager->dpy, windowId, CWX | CWY, &changes);
}

void NativeControlImpl::setDims(WDims dims) {
	XResizeWindow(resourceManager->dpy, windowId, dims.width, dims.height);
}

void NativeControlImpl::setRect(Rect rect) {
	XWindowChanges changes;
	changes.x = rect.left;
	changes.y = rect.top;
	changes.width = rect.width();
	changes.height = rect.height();
	XConfigureWindow(resourceManager->dpy, windowId, CWX | CWY | CWWidth | CWHeight, &changes);
}

void NativeControlImpl::show() {
	XMapWindow(resourceManager->dpy, windowId);
}

void NativeControlImpl::hide() {
	XUnmapWindow(resourceManager->dpy, windowId);
}

bool NativeControlImpl::visible() const {
	XWindowAttributes attrs;
	XGetWindowAttributes(resourceManager->dpy, windowId, &attrs);
	return attrs.map_state != IsUnmapped;
}

void NativeControlImpl::bringToFront() {
	XRaiseWindow(resourceManager->dpy, windowId);
}

void NativeControlImpl::sendToBack() {
	XLowerWindow(resourceManager->dpy, windowId);
}

bool NativeControlImpl::enabled() const {
	return windowEnabled;
}

void NativeControlImpl::enable() {
	windowEnabled = true;
}

void NativeControlImpl::disable() {
	// TODO disable all events
	windowEnabled = false;
}

NativeString NativeControlImpl::getText() const {
	return {};
}

void NativeControlImpl::setText(const NativeString & text) {
}

Rect NativeControlImpl::rect() const {
	XWindowAttributes attrs;
	XGetWindowAttributes(resourceManager->dpy, windowId, &attrs);
	return Rect::closed(Point(attrs.x, attrs.y), WDims(attrs.width, attrs.height));
}

ClipboardType NativeControlImpl::copyToClipboard() const {
	return ClipboardType::nothing;
}

Canvas & NativeControlImpl::canvas() {
	return windowCanvas;
}

void NativeControlImpl::repaint() {
	XClearArea(resourceManager->dpy, windowId, 0, 0, 0, 0, True);
}

void NativeControlImpl::invalidate(Rect const & rect) {
	XClearArea(resourceManager->dpy, windowId, rect.left, rect.top, rect.dims().width, rect.dims().height, True);
}

void NativeControlImpl::setCursor(Cursor cursor) {
	this->cursor = cursor;
	nativeCursor.reset(XCreateFontCursor(resourceManager->dpy, toShape(cursor)));
	XDefineCursor(resourceManager->dpy, windowId, nativeCursor.get());
}

void NativeControlImpl::setBackground(RGBColor const & color) {
	XSetWindowBackground(resourceManager->dpy, windowId, rgb32(color));
	backgroundColor = color;
}

Point NativeControlImpl::screenToClient(Point const & point) const {
	Point result;
	::Window child;
	XTranslateCoordinates(resourceManager->dpy,
						  resourceManager->root(), windowId,
						  point.x, point.y,
						  &result.x, &result.y, &child);
	return result;
}

::Window NativeControlImpl::getParentHandle() const {
	return parentWindowId;
	/*
	::Window root_return;
	::Window parent_return;
	::Window * children_return;
	unsigned int nchildren_return;
	XQueryTree(resourceManager->dpy, windowId,
		&root_return, &parent_return, &children_return, &nchildren_return);
	if( children_return ) {
		XFree(children_return);
	}
	return parent_return;
	*/
}

ControlParams NativeControlImpl::getControlData() const {
	auto rectangle = rect();

	ControlParams result = ControlParams()
		.start(rectangle.topLeft())
		.dims(rectangle.dims())
		.text(getText())
		.visible(visible())
		.cursor(cursor);
	// no scrollbar status?

	if( backgroundColor ) {
		result.backgroundColor(*backgroundColor);
	}
	return result;
}

void NativeControlImpl::addCustomControlImpl(std::shared_ptr<CustomControlImpl> const & controlImpl) {
	customControls.push_back(controlImpl);
}

// callbacks & messages

HandleMouseButton NativeControlImpl::onMouseDown(HandleMouseButton callback) {
	return setCallback(this, cbMouseDown, callback);
}

HandleMouseButton NativeControlImpl::onMouseUp(HandleMouseButton callback) {
	return setCallback(this, cbMouseUp, callback);
}

HandleMouseMove NativeControlImpl::onMouseEnter(HandleMouseMove callback) {
	return setCallback(this, cbMouseEnter, callback);
}

HandleMouseMove NativeControlImpl::onMouseOver(HandleMouseMove callback) {
	return setCallback(this, cbMouseOver, callback);
}

HandleMouseMove NativeControlImpl::onMouseLeave(HandleMouseMove callback) {
	return setCallback(this, cbMouseLeave, callback);
}

ProcessKeyEvent NativeControlImpl::onKeyDown(ProcessKeyEvent callback) {
	return setCallback(this, cbKeyDown, callback);
}

ProcessKeyEvent NativeControlImpl::onKeyUp(ProcessKeyEvent callback) {
	return setCallback(this, cbKeyUp, callback);
}

HandlePaint NativeControlImpl::onPaint(HandlePaint callback) {
	return setCallback(this, cbPaint, callback);
}

ProcessKeypress NativeControlImpl::onKeypress(ProcessKeypress callback) {
	return setCallback(this, cbKeypress, callback);
}

void NativeControlImpl::processMessage(XEvent & e) {
	//log::info("window ",e.xany.window," got event ",xEventToStr(e.type));

	switch(e.type) {
		case Expose: {
			auto & data = e.xexpose;
			auto rect = Rect::closed(Point(data.x, data.y);
			executeCallback(this, cbPaint, canvas(),
							rect, WDims(data.width, data.height)));
			// draw all custom controls, if any
			for( auto && customControl : customControls ) {
				if( customControl->rect.intersect(rect) ) {
					customControl->draw(canvas(),rect);
				}
			}
			// TODO check windowEnabled and gray the window over
		}
			break;

		case ButtonPress: {
			auto & data = e.xbutton;
			executeCallback(this, cbMouseDown, toMouseEvent(data), Point(data.x, data.y));
		}
			break;
		case ButtonRelease: {
			auto & data = e.xbutton;
			executeCallback(this, cbMouseUp, toMouseEvent(data), Point(data.x, data.y));
		}
			break;

		case KeyPress:
			keyPressEvent(e);
			break;
		case KeyRelease:
			keyReleaseEvent(e);
			break;

		case EnterNotify: {
			auto & data = e.xcrossing;
			executeCallback(this, cbMouseEnter, Point(data.x, data.y));
		}
			break;
		case LeaveNotify: {
			auto & data = e.xcrossing;
			executeCallback(this, cbMouseLeave, Point(data.x, data.y));
		}
			break;
		case MotionNotify: {
			auto & data = e.xmotion;
			executeCallback(this, cbMouseOver, Point(data.x, data.y));
		}
			break;
	}
}

void NativeControlImpl::keyPressEvent(XEvent & e) {
	auto it_kd = cbKeyDown.find(this);
	auto it_kp = cbKeypress.find(this);
	if( it_kd == cbKeyDown.end() && it_kp == cbKeypress.end()) return;

	// get event info
	auto & data = e.xkey;
	char chars[20];
	KeySym keySym;
	XLookupString(&data, chars, 20, &keySym, 0);

	// forward to cbKeyDown if defined
	if( it_kd != cbKeyDown.end()) {
		auto keyPressed = fromKeySym(keySym);
#ifndef NDEBUG
		if( keyPressed == k_NONE ) {
			log::error("untranslated KeySym. code: ", data.keycode, " key: ", keySym);
		}
#endif
		auto newKey = it_kd->second(keyPressed);
		if( newKey != k_NONE ) {
			if( newKey != keyPressed ) {
				data.keycode = toKeyCode(newKey);
			}
			// TODO forward the event to child windows
		}
	}

	// forward to cbKeyPress if defined
	if( it_kp != cbKeypress.end()) {
		auto ch = chars[0];
		if( ch != '\0' ) {
			auto newCh = it_kp->second(ch);
			if( newCh != '\0' ) {
				// TODO forward the event to child windows (somehow)
			}
		}
		/*
		for( auto cp = &chars[0]; cp != nullptr; ++cp ) {
			auto newCh = it_kp->second(*cp);
			if( newCh != '\0' ) {
				// TODO forward the event to child windows (somehow)
			}
		}
		*/
	}
}

void NativeControlImpl::keyReleaseEvent(XEvent & e) {
	auto it_ku = cbKeyUp.find(this);
	if( it_ku == cbKeyUp.end()) return;

	// get event info
	auto & data = e.xkey;
	KeySym keySym;
	XLookupString(&data, 0, 0, &keySym, 0);

	auto keyReleased = fromKeySym(keySym);
	auto newKey = it_ku->second(keyReleased);
	if( newKey != k_NONE ) {
		if( newKey != keyReleased ) {
			data.keycode = toKeyCode(newKey);
		}
		// TODO forward the event to child windows
	}
}

}
