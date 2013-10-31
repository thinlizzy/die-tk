#include "WindowImplWin32.h"
#include <algorithm>
#include <stdexcept>

#include "ConvertersWin32.h"
#include "ScopedObjects.h"
#include "ResourceManager.h"
#include "CallbackUtils.h"

#include "../log.h"

#include "../trace.h"

namespace {

std::shared_ptr<tk::NativeControlImpl> nullControl;

}

namespace tk {

ControlCallbackMap<AllowOperation> cbClose;
ControlCallbackMap<ProcessResize> cbResize;
ControlCallbackMap<HandleEvent> cbUserEvent;

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
	wc.lpszClassName = L"die-tk-window";
	RegisterClassW(&wc);
}

WindowClass WindowImpl::windowClass;



tk::WDims windowPayload(8,27);

DWORD stateToWinStyle(int state)
{
	DWORD result = 0;
	if( state & ws_visible ) {
        result |= WS_VISIBLE;
    }
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

	hWnd = createWindow(Point(x,y),dims,params.text_.wstr.c_str(),windowClass.wc.lpszClassName,stateToWinStyle(params.initialState));

	rect_ = convertRect(windowRect());
}

HWND WindowImpl::createWindow(Point pos, WDims dims, wchar_t const windowname[], wchar_t const classname[], DWORD style)
{
	HWND hWnd = CreateWindowExW(
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
		throw std::runtime_error(log::nativeErrorString());
	}
    return hWnd;
}

WindowImpl::~WindowImpl()
{
	if( state_ != ws_destroyed ) {
		// DestroyWindow(hWnd);
		// PostMessage(hWnd,WM_CLOSE,0,0);
		PostMessage(hWnd,WM_DESTROY,0,0);
	}
    removeFromCb(this,cbClose);
    removeFromCb(this,cbResize);
    removeFromCb(this,cbUserEvent);
}

WindowImpl * WindowImpl::clone() const
{
    if( state_ == ws_destroyed ) return 0;
    
    return new WindowImpl(WindowParams()
            .start(rect().pos())
            .dims(rect().dims())
            .text(getText())
            .states(state_)
    );
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
    resourceManager.registerControl(control);
}

void WindowImpl::unregisterControl(std::shared_ptr<NativeControlImpl> control)
{
    if( controls.erase(control->hWnd) > 0 ) {
        resourceManager.unregisterControl(control);
        DestroyWindow(control->hWnd);
    }
}

// callbacks & messages

AllowOperation WindowImpl::onClose(AllowOperation callback)
{
    return setCallback(this,cbClose,callback);    
}

ProcessResize WindowImpl::onResize(ProcessResize callback)
{
    return setCallback(this,cbResize,callback);    
}

HandleEvent WindowImpl::onUserEvent(HandleEvent callback)
{
    return setCallback(this,cbUserEvent,callback);    
}

optional<LRESULT> WindowImpl::processMessage(UINT message, WPARAM & wParam, LPARAM & lParam)
{
    if( auto result = NativeControlImpl::processMessage(message,wParam,lParam) ) return result;

	switch( message ) {
		case WM_DESTROY:
			state_ = ws_destroyed;
			PostQuitMessage(0);			// TODO really need that? I am not processing WM_QUIT anymore
			break;
            
		case WM_CLOSE: {
            auto canClose = findExec<bool>(this,cbClose);
            if( canClose ) {
                if( *canClose == false ) return 0;
            }
            
			hide();
			return 0;            
		} break;

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
            
            auto resNewDims = findExec<WDims>(this,cbResize,newDims);
            if( resNewDims ) {
                // TODO verify the need of creating on_minimize and on_maximize callbacks also
                lParam = WDimsToLParam(*resNewDims);
            }            
		} break;
        
		case WM_COMMAND: {
			if( auto control = resourceManager.findControl(HWND(lParam)) ) {
                return control->processNotification(WM_COMMAND,HIWORD(wParam),wParam,lParam);
			}
		} break;

		case WM_NOTIFY: {
            auto hdr = reinterpret_cast<LPNMHDR>(lParam);
			if( auto control = resourceManager.findControl(hdr->hwndFrom) ) {
                return control->processNotification(WM_NOTIFY,hdr->code,wParam,lParam);
			}
		} break;

		default:
			if( message >= WM_USER ) {
                if( findExec(this,cbUserEvent,toUserEvent(message,lParam)) ) {
                    return 0;
                }
			}
	}

	return optional<LRESULT>();
}

}
