#include "WindowImplWin32.h"
#include "../Window.h"
#include "ConvertersWin32.h"
#include "ScopedObjects.h"
#include <algorithm>
#include "ApplicationWin32.h"
#include <stdexcept>

#include "../trace.h"

namespace {

std::shared_ptr<tk::NativeControlImpl> nullControl;

}

namespace tk {

WindowClass::WindowClass()
{
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = WndProc;   // from ApplicationWin32
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = GetModuleHandle(NULL);
	wc.hIcon = LoadIcon(NULL,IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL,IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = "die-tk-window";
	RegisterClass(&wc);
}

WindowClass WindowImpl::windowClass;



tk::WDims windowPayload(8,27);

DWORD stateToWinStyle(int state)
{
	DWORD result = 0;
	if( state & ws_visible ) result |= WS_VISIBLE;
	if( state & ws_minimized ) {
		result |= WS_MINIMIZE;
	} else
	if( state & ws_maximized ) {
		result |= WS_MAXIMIZE;
	}

	return result;
}


WindowImpl::WindowImpl(WindowParams const & params):
	state_(params.initialState)
{
	// create window
	int x,y;
	WDims dims;
	if( params.isDefaultPos() ) {
		x = y = CW_USEDEFAULT;
	} else {
		x = params.start_.x;
		y = params.start_.y;
	}
	if( params.isDefaultDims() ) {
		dims.width = dims.height = CW_USEDEFAULT;
	} else {
		dims = params.dims_ + windowPayload;
	}

	createWindow(Point(x,y),dims,params.text_.c_str(),windowClass.wc.lpszClassName,stateToWinStyle(params.initialState));

	rect_ = convertRect(windowRect());
}

void WindowImpl::createWindow(Point pos, WDims dims, char const windowname[], char const classname[], DWORD style)
{
	hWnd = CreateWindowEx(
		WS_EX_APPWINDOW,
		classname, windowname,
//		WS_CLIPCHILDREN | WS_OVERLAPPEDWINDOW | style,
		WS_OVERLAPPEDWINDOW | style,
		pos.x, pos.y,
		dims.width, dims.height,
		NULL, NULL,
		GetModuleHandle(NULL), NULL
	);
	if( hWnd == NULL ) {
		auto error = errorMessage(GetLastError());
		throw std::runtime_error(error);
	}
}

WindowImpl::~WindowImpl()
{
	if( state_ != ws_destroyed ) {
		// DestroyWindow(hWnd);
		// PostMessage(hWnd,WM_CLOSE,0,0);
		PostMessage(hWnd,WM_DESTROY,0,0);
	}
}


void WindowImpl::setRect(Rect rect)
{
	rect_.left = rect.left;
	rect_.top = rect.top;
	setDims(rect.dims());
}

void WindowImpl::setDims(WDims dims)
{
	rect_ = rect_.resize(dims);
	dims += windowPayload;
	SetWindowPos(hWnd,0,0,0,dims.width,dims.height,SWP_NOMOVE | SWP_NOZORDER);
}


int WindowImpl::state() const { return state_; }

void WindowImpl::show()
{
	state_ |= ws_visible;
	NativeControlImpl::show();
}

void WindowImpl::hide()
{
	state_ &= ~ws_visible;
	NativeControlImpl::hide();
}

void WindowImpl::clear(RGBColor const & color)
{
	canvasImpl.setBrush(color);
	canvasImpl.setPen(color);
	canvasImpl.fillRect(windowRect());
}


RECT WindowImpl::windowRect() const
{
	RECT rect;
	GetClientRect(hWnd,&rect);
	return rect;
}

std::shared_ptr<NativeControlImpl> WindowImpl::findControl(HWND handle)
{
	auto it = controls.find(handle);
	if( it == controls.end() ) return nullControl;

	return it->second;
}

void WindowImpl::registerControl(std::shared_ptr<NativeControlImpl> control)
{
	controls[control->hWnd] = control;
}

bool WindowImpl::processMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
    if( NativeControlImpl::processMessage(message,wParam,lParam) ) return true;

	switch( message ) {
		case WM_DESTROY:
			state_ = ws_destroyed;
			PostQuitMessage(0);			// TODO really need that? I am not processing WM_QUIT anymore
			break;
		case WM_CLOSE:
			hide();
			return true;
			break;

		case WM_SIZE: {
			WDims newDims = lParamToWDims(lParam);
			rect_ = rect_.resize(newDims);
			switch(wParam) {
				case SIZE_MINIMIZED:
					state_ |= ws_minimized;
					state_ &= ~ws_maximized;
					break;
				case SIZE_MAXIMIZED:
					state_ &= ~ws_minimized;
					state_ |= ws_maximized;
					break;
				case SIZE_RESTORED:
					state_ &= ~(ws_minimized | ws_maximized);
					break;
			}
			} break;
	}

	return false;
}

}
