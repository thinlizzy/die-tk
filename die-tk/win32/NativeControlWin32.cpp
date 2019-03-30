#define WINVER 0x0500

#include "NativeControlWin32.h"
#include <winuser.h>
#include <stdexcept>
#include "ConvertersWin32.h"
#include "ResourceManager.h"

#include "../log.h"
#include "../trace.h"

namespace tk {

ControlCallbackMap<HandlePaint> cbPaint;
ControlCallbackMap<HandleMouseButton> cbMouseDown, cbMouseUp;
ControlCallbackMap<HandleMouseMove> cbMouseEnter, cbMouseOver, cbMouseLeave;
ControlCallbackMap<ProcessKeyEvent> cbKeyDown, cbKeyUp;
ControlCallbackMap<ProcessKeypress> cbKeypress;

DWORD scrollbarToWinStyle(Scrollbar sb) {
	switch(sb) {
		case Scrollbar::both:
			return WS_HSCROLL | WS_VSCROLL;
		case Scrollbar::horizontal:
			return WS_HSCROLL;
		case Scrollbar::vertical:
			return WS_VSCROLL;
		default:
			return 0;
	}
}

Scrollbar getScrollbarStatus(HWND hWnd) {
	int const state_system_invisible = 0x8000; // STATE_SYSTEM_INVISIBLE is not defined...

	Scrollbar sb = Scrollbar::none;
	SCROLLBARINFO sbi;
	sbi.cbSize = sizeof(SCROLLBARINFO);
	if( GetScrollBarInfo(hWnd, OBJID_HSCROLL, &sbi)) {
		if( (sbi.rgstate[0] & state_system_invisible) == 0 ) {
			sb = Scrollbar::horizontal;
		}
	}
	if( GetScrollBarInfo(hWnd, OBJID_VSCROLL, &sbi)) {
		if( (sbi.rgstate[0] & state_system_invisible) == 0 ) {
			sb = sb == Scrollbar::none ? Scrollbar::vertical : Scrollbar::both;
		}
	}
	return sb;
}


NativeControlImpl::NativeControlImpl():
	cursor(Cursor::defaultCursor),
	trackingMouse(false) {
}

// parent.getImpl().hWnd
NativeControlImpl::NativeControlImpl(
		HWND parentHwnd, ControlParams const & params, wchar_t const classname[], DWORD style):
	cursor(params.cursor_) {
	hWnd = CreateWindowW(
		classname,
		NULL,
		WS_CHILD | WS_CLIPSIBLINGS | style | (params.visible_ ? WS_VISIBLE : 0) | scrollbarToWinStyle(params.scrollbar_),
		params.start_.x, params.start_.y,
		params.dims_.width, params.dims_.height,
		parentHwnd,
		NULL,
		GetModuleHandle(NULL),
		NULL
	);
	if( hWnd == NULL ) {
		throw std::runtime_error(log::nativeErrorString());
	}

	if( params.backgroundColor_ ) {
		setBackground(*params.backgroundColor_);
	}
}

NativeControlImpl::~NativeControlImpl() {
	removeFromCb(this,cbPaint);
	removeFromCb(this,cbMouseDown);
	removeFromCb(this,cbMouseUp);
	removeFromCb(this,cbMouseEnter);
	removeFromCb(this,cbMouseOver);
	removeFromCb(this,cbMouseLeave);
	removeFromCb(this,cbKeyDown);
	removeFromCb(this,cbKeyUp);
	removeFromCb(this,cbKeypress);
}

Canvas & NativeControlImpl::canvas() {
	canvasImpl.getDC(hWnd);
	return canvasImpl;
}

RECT NativeControlImpl::getClientRect() const {
	RECT rc;
	GetClientRect(hWnd, &rc);
	MapWindowPoints(hWnd, GetParent(hWnd), (LPPOINT) &rc, 2);
	return rc;
}

Rect NativeControlImpl::rect() const {
	RECT rc = getClientRect();
	--rc.right;
	--rc.bottom;
	return convertRect(rc);
}

void NativeControlImpl::setPos(Point pos) {
	SetWindowPos(hWnd, 0, pos.x, pos.y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
}

void NativeControlImpl::setDims(WDims dims) {
	SetWindowPos(hWnd, 0, 0, 0, dims.width, dims.height, SWP_NOMOVE | SWP_NOZORDER);
}

void NativeControlImpl::setRect(Rect rect) {
	SetWindowPos(hWnd, 0, rect.left, rect.top, rect.dims().width, rect.dims().height, SWP_NOZORDER);
}

bool NativeControlImpl::enabled() const {
	return IsWindowEnabled(hWnd);
}

void NativeControlImpl::enable() {
	EnableWindow(hWnd, true);
}

void NativeControlImpl::disable() {
	EnableWindow(hWnd, false);
}

bool NativeControlImpl::visible() const {
	return IsWindowVisible(hWnd);
}

void NativeControlImpl::show() {
	ShowWindow(hWnd, SW_SHOW);
}

void NativeControlImpl::hide() {
	ShowWindow(hWnd, SW_HIDE);
}

void NativeControlImpl::bringToFront() {
	SetWindowPos(hWnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
}

void NativeControlImpl::sendToBack() {
	SetWindowPos(hWnd, HWND_BOTTOM, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
}

void NativeControlImpl::setText(NativeString const & text) {
	SetWindowTextW(hWnd, text.wstr.c_str());
}

ClipboardType NativeControlImpl::copyToClipboard() const {
	ClipboardType result = ClipboardType::nothing;
	auto len = GetWindowTextLengthW(hWnd);
	if( len == 0 ) return result;

	if( !OpenClipboard(hWnd)) {
		log::error("OpenClipboard failed for hWnd ", hWnd);
		return result;
	}

	if( EmptyClipboard()) {
		HGLOBAL hglbCopy = GlobalAlloc(GMEM_MOVEABLE, (len + 1) * sizeof(wchar_t));
		if( hglbCopy != NULL ) {
			auto lptstrCopy = reinterpret_cast<wchar_t *>(GlobalLock(hglbCopy));
			GetWindowTextW(hWnd, lptstrCopy, len + 1);
			GlobalUnlock(hglbCopy);

			if( SetClipboardData(CF_UNICODETEXT, hglbCopy) != NULL ) {
				result = ClipboardType::text;
			} else {
				log::error("SetClipboardData failed for hWnd ", hWnd);
			}
		}
	} else {
		log::error("EmptyClipboard failed for hWnd ", hWnd);
	}

	CloseClipboard();
	return result;
}


NativeString NativeControlImpl::getText() const {
	auto len = GetWindowTextLengthW(hWnd);
	if( len == 0 ) return NativeString();

	NativeString result;
	result.wstr.resize(len + 1, 0);
	GetWindowTextW(hWnd, &result.wstr[0], len + 1);
	result.wstr.pop_back();
	return result;
}

SIZE NativeControlImpl::getTextDims() {
	auto sdc = getDC();
	auto text = getText();
	SIZE size;
	GetTextExtentPoint32W(sdc.hdc, text.wstr.data(), text.wstr.length(), &size);
	return size;
}

void NativeControlImpl::repaint() {
	InvalidateRect(hWnd, 0, true);
}

void NativeControlImpl::invalidate(Rect const & rect) {
	auto winRect = convertOpenRect(rect);
	InvalidateRect(hWnd, &winRect, false);
}

scoped::DC NativeControlImpl::getDC() const {
	return scoped::DC(hWnd);
}

// TODO call winapi SetCursor() if mouse is hovering the control
void NativeControlImpl::setCursor(Cursor cursor) {
	this->cursor = cursor;
}

void NativeControlImpl::setBackground(RGBColor const & color) {
	this->backgroundColor = color;
	backgroundBrush.reset(CreateSolidBrush(colorWin(backgroundColor)));
}

Point NativeControlImpl::screenToClient(Point const & point) const {
	auto p = convertPoint(point);
	ScreenToClient(hWnd, &p);
	return convertPoint(p);
}

HWND NativeControlImpl::getParentHandle() const {
	HWND result = GetParent(hWnd);
	if( result == NULL ) {
		log::error("getParentHwnd() failed for hWnd ", hWnd);
	}
	return result;
}

// TODO add enabled state
ControlParams NativeControlImpl::getControlData() const {
	ControlParams result = ControlParams()
		.start(rect().topLeft())
		.dims(rect().dims())
		.text(getText())
		.visible(visible())
		.scrollbar(getScrollbarStatus(hWnd))
		.cursor(cursor);
	if( backgroundBrush ) {
		result.backgroundColor(backgroundColor);
	}
	return result;
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

ProcessKeypress NativeControlImpl::onKeypress(ProcessKeypress callback) {
	return setCallback(this, cbKeypress, callback);
}

HandlePaint NativeControlImpl::onPaint(HandlePaint callback) {
	return setCallback(this, cbPaint, callback);
}

#define GETCB(mapname, varname) \
auto varname = fetchCallback(this,mapname); \
if( varname == nullptr ) return optional<LRESULT>();

optional<LRESULT> NativeControlImpl::processMessage(UINT message, WPARAM & wParam, LPARAM & lParam) {
	optional<LRESULT> result;
	switch(message) {
		case WM_SETCURSOR:
			if( cursor != Cursor::defaultCursor ) {
				result = 0;
				ResourceManagerSingleton resourceManager;
				SetCursor(resourceManager->cursors[int(cursor)]);
			}
			break;
		case WM_ERASEBKGND:
			if( backgroundBrush ) {
				result = 0;
				HDC hdc = (HDC) wParam;
				RECT rc;
				GetClientRect(hWnd, &rc);
				FillRect(hdc, &rc, backgroundBrush.get());
			}
			break;

		case WM_PAINT: {
			GETCB(cbPaint, on_paint);
			scoped::PaintSection sps(hWnd);
			CanvasImpl canvas(sps.ps.hdc);
			auto rect = convertRect(sps.ps.rcPaint);
			on_paint(canvas, rect);
			return 0;
		}
			break;

		case WM_SYSKEYDOWN:
		case WM_KEYDOWN: {
			GETCB(cbKeyDown, on_key_down);
			WindowKey key = on_key_down(fromWindowsKey(wParam, lParam));
			if( key == k_NONE ) return 0;

			wParam = toWindowsKey(key);
		}
			break;
		case WM_SYSKEYUP:
		case WM_KEYUP: {
			GETCB(cbKeyUp, on_key_up);
			WindowKey key = on_key_up(fromWindowsKey(wParam, lParam));
			if( key == k_NONE ) return 0;

			wParam = toWindowsKey(key);
		}
			break;

		case WM_CHAR: {
			GETCB(cbKeypress, on_keypress);
			char key = on_keypress(wParam);
			if( key == 0 ) return 0;

			wParam = key;
		}
			break;

		case WM_LBUTTONDOWN:
		case WM_RBUTTONDOWN:
		case WM_MBUTTONDOWN:
			return doMouseButton(cbMouseDown, message, wParam, lParam);
		case WM_LBUTTONUP:
		case WM_RBUTTONUP:
		case WM_MBUTTONUP:
			return doMouseButton(cbMouseUp, message, wParam, lParam);

		case WM_MOUSEMOVE:
			if( trackingMouse ) return doMouseMove(cbMouseOver, lParam);

			trackingMouse = true;
			{
				TRACKMOUSEEVENT tme;
				tme.cbSize = sizeof(tme);
				tme.dwFlags = TME_LEAVE;
				tme.hwndTrack = hWnd;
				if( TrackMouseEvent(&tme) == 0 ) {
					log::error("TrackMouseEvent() failed for hWnd ", hWnd);
				}
			}
			return doMouseMove(cbMouseEnter, lParam);

		case WM_MOUSELEAVE:
			trackingMouse = false;
			return doMouseMove(cbMouseLeave, lParam);
	}
	return result;
}

optional<LRESULT> NativeControlImpl::doMouseButton(
	ControlCallbackMap<HandleMouseButton> & callbacks, UINT message, WPARAM wParam, LPARAM lParam) {
	GETCB(callbacks, callback);
	auto p = lParamToPoint(lParam);
	auto mouseEvent = toMouseEvent(message, wParam);
	callback(mouseEvent, p);
	return 0;
}

optional<LRESULT> NativeControlImpl::doMouseMove(ControlCallbackMap<HandleMouseMove> & callbacks, LPARAM lParam) {
	GETCB(callbacks, callback);
	auto p = lParamToPoint(lParam);
	callback(p);
	return 0;
}

optional<LRESULT>
NativeControlImpl::processNotification(UINT message, UINT notification, WPARAM wParam, LPARAM lParam) {
	return optional<LRESULT>();
}

}
