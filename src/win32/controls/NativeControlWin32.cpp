#include "NativeControlWin32.h"
#include "../ConvertersWin32.h"
#include "../ApplicationWin32.h"

#include "../../trace.h"
#include <stdexcept>

namespace tk {

DWORD scrollbarToWinStyle(Scrollbar sb)
{
	switch(sb) {
	    case sb_both: return WS_HSCROLL + WS_VSCROLL;
	    case sb_horizontal: return WS_HSCROLL;
	    case sb_vertical: return WS_VSCROLL;
	    default: return 0;
	}
}


NativeControlImpl::NativeControlImpl():
    cursor(cur_default),
    backgroundColor(RGBColor())
{
}

NativeControlImpl::NativeControlImpl(HWND parent_hWnd, ControlParams const & params, char const classname[], DWORD style):
    cursor(cur_default),
    backgroundColor(RGBColor()),
	rect_(Rect::closed(params.start_,params.dims_))
{
	hWnd = CreateWindow(
		classname, NULL,
        style | scrollbarToWinStyle(params.scrollbar_) |
            WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS,
		params.start_.x, params.start_.y,
		params.dims_.width, params.dims_.height,
		parent_hWnd,
		NULL,
		GetModuleHandle(NULL), NULL
	);
	if( hWnd == NULL ) {
		auto error = errorMessage(GetLastError());
		throw std::runtime_error(error);
	}
}

NativeControlImpl::~NativeControlImpl()
{}

Canvas & NativeControlImpl::canvas()
{
	canvasImpl.getDC(hWnd);
	return canvasImpl;
}

int NativeControlImpl::x() const
{
	return pos().x;
}

int NativeControlImpl::y() const
{
	return pos().y;
}

Point NativeControlImpl::pos() const
{
	return rect().pos();
}

int NativeControlImpl::width() const
{
	return dims().width;
}

int NativeControlImpl::height() const
{
	return dims().height;
}

WDims NativeControlImpl::dims() const
{
	return rect().dims();
}

Rect NativeControlImpl::rect() const
{
	return rect_;
}


void NativeControlImpl::setPos(Point pos)
{
	SetWindowPos(hWnd,0,pos.x,pos.y,0,0,SWP_NOSIZE | SWP_NOZORDER);
	rect_ = rect().move(pos);
}

void NativeControlImpl::setDims(WDims dims)
{
	SetWindowPos(hWnd,0,0,0,dims.width,dims.height,SWP_NOMOVE | SWP_NOZORDER);
	rect_ = rect().resize(dims);
}

void NativeControlImpl::setRect(Rect rect)
{
	rect_ = rect;
	SetWindowPos(hWnd,0,rect.left,rect.top,rect.dims().width,rect.dims().height,SWP_NOZORDER);
}

bool NativeControlImpl::enabled() const
{
	return IsWindowEnabled(hWnd);
}

void NativeControlImpl::enable()
{
	EnableWindow(hWnd,true);
}

void NativeControlImpl::disable()
{
	EnableWindow(hWnd,false);
}

bool NativeControlImpl::visible() const
{
    return IsWindowVisible(hWnd);
}

void NativeControlImpl::show()
{
	ShowWindow(hWnd,SW_SHOW);
}

void NativeControlImpl::hide()
{
	ShowWindow(hWnd,SW_HIDE);
}

void NativeControlImpl::bringToFront()
{
	SetWindowPos(hWnd,HWND_TOP,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
}

void NativeControlImpl::sendToBack()
{
	SetWindowPos(hWnd,HWND_BOTTOM,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
}

void NativeControlImpl::setText(std::string const & text)
{
	SetWindowText(hWnd,text.c_str());
}

std::string NativeControlImpl::getText() const
{
	auto len = GetWindowTextLength(hWnd);
	if( len == 0 ) return std::string();

	std::string result(len+1,0);
	GetWindowText(hWnd,&result[0],len+1);
	result.resize(len);
	return result;
}

SIZE NativeControlImpl::getTextDims()
{
	auto sdc = getDC();
	auto text = getText();
	SIZE size;
	GetTextExtentPoint32(sdc.hdc,text.data(),text.length(),&size);
	return size;
}

void NativeControlImpl::repaint()
{
	InvalidateRect(hWnd,0,true);
}

scoped::DC NativeControlImpl::getDC()
{
	return scoped::DC(hWnd);
}

bool NativeControlImpl::processMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
    bool handled = false;
    switch(message) {
        case WM_SETCURSOR:
            if( cursor != cur_default ) {
                handled = true;
                SetCursor(globalAppImpl->cursors[cursor]);
            }
            break;
        case WM_ERASEBKGND:
            if( backgroundColor != RGBColor() ) {
                handled = true;
                HDC hdc = (HDC) wParam;
                RECT rc;
                GetClientRect(hWnd, &rc);
                scoped::Brush brush(CreateSolidBrush(colorWin(backgroundColor)));
                FillRect(hdc, &rc, brush.get());
            }
            break;
    }
	return handled;
}

bool NativeControlImpl::processNotification(UINT message, UINT notification, WPARAM wParam, LPARAM lParam)
{
    return false;
}

void NativeControlImpl::clear(RGBColor const & color)
{
    auto & canvas1 = canvas();
    canvas1.setBrush(color);
	canvas1.setPen(color);
	canvas1.fillRect(Rect::closed(Point(0,0),dims()));
}

void NativeControlImpl::setCursor(Cursor cursor)
{
    this->cursor = cursor;
}

void NativeControlImpl::setBackground(RGBColor const & color)
{
    this->backgroundColor = color;
}

Point NativeControlImpl::screenToClient(Point const & point) const
{
    auto p = convertPoint(point);
    ScreenToClient(hWnd,&p);
    return convertPoint(p);
}

}
