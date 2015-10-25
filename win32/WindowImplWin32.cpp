#include "WindowImplWin32.h"
#include <algorithm>
#include <stdexcept>

#include "ConvertersWin32.h"
#include "ScopedObjects.h"
#include "ResourceManager.h"
#include "../src/CallbackUtils.h"
#include "../src/log.h"
#include "../src/trace.h"

namespace {

std::shared_ptr<tk::NativeControlImpl> nullControl;

}

namespace tk {

template<typename T> using WindowCallbackMap = CallbackMap<WindowImpl *, T>; 

WindowCallbackMap<AllowOperation> cbClose;
WindowCallbackMap<ProcessResize> cbResize;
WindowCallbackMap<HandleResize> cbAfterResize;
WindowCallbackMap<HandleEvent> cbUserEvent;

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
		dims = params.dims_ + framePayload(DWORD(-1));  // TODO use a standard payload
	}

	hWnd = createWindow(Point(x,y),dims,params.text_.wstr.c_str(),windowClass.wc.lpszClassName,stateToWinStyle(params.initialState));
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
    removeFromCb(this,cbClose);
    removeFromCb(this,cbResize);
    removeFromCb(this,cbUserEvent);
    for( auto & component : components ) {
        component->unregister();
    }
    
    DestroyWindow(hWnd);
}

WindowImpl * WindowImpl::clone() const
{
    return new WindowImpl(WindowParams()
            .start(rect().topLeft())
            .dims(rect().dims())
            .text(getText())
            .states(state_)
    );
}

WDims WindowImpl::componentsPayload()
{
    WDims result;
    for( auto & component : components ) {
        result += component->payload();
    }
    
    return result;
}

WDims WindowImpl::framePayload(DWORD style)
{
    WDims result;
    if( style & WS_CAPTION ) result.height += 19;      // TODO get title bar size
    if( style & WS_BORDER ) result += WDims(4,4);      // TODO get border size
    if( style & WS_THICKFRAME ) result += WDims(4,4);  // TODO get thick frame size
    return result;
}

WDims WindowImpl::windowPayload()
{
    return framePayload(GetWindowLong(hWnd,GWL_STYLE)) + componentsPayload();
}

void WindowImpl::setRect(Rect rect)
{
    rect = rect.resize(rect.dims() + windowPayload());
    NativeControlImpl::setRect(rect);	
}

void WindowImpl::setDims(WDims dims)
{
	dims += windowPayload();
    NativeControlImpl::setDims(dims);
}

int WindowImpl::state() const { return state_; }

void WindowImpl::setBorders(bool value)
{
    auto dims = rect().dims();
    DWORD style = GetWindowLong(hWnd,GWL_STYLE);
    if( value ) {
        style |= WS_BORDER | WS_CAPTION | WS_THICKFRAME;
    } else {
        style &= ~(WS_BORDER | WS_CAPTION | WS_THICKFRAME);
    }
    SetWindowLong(hWnd,GWL_STYLE,style);
//    SetWindowPos(hWnd,0,0,0,0,0,SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_FRAMECHANGED);
//    setDims(dims);
    dims += componentsPayload() + framePayload(style);
    SetWindowPos(hWnd,0,0,0,dims.width,dims.height,SWP_NOMOVE | SWP_NOZORDER | SWP_FRAMECHANGED);
}

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

HandleResize WindowImpl::afterResize(HandleResize callback)
{
    return setCallback(this,cbAfterResize,callback);
}

HandleEvent WindowImpl::onUserEvent(HandleEvent callback)
{
    return setCallback(this,cbUserEvent,callback);    
}

optional<LRESULT> WindowImpl::processMessage(UINT message, WPARAM & wParam, LPARAM & lParam)
{
    if( auto result = NativeControlImpl::processMessage(message,wParam,lParam) ) return result;

	switch( message ) {
		case WM_CLOSE: {
            auto canClose = findExec(this,cbClose);
            if( canClose ) {
                if( *canClose == false ) return 0;
            }
            
			hide();
			return 0;            
		} break;

        // TODO verify the need of creating on_minimize and on_maximize callbacks also
		case WM_SIZE: {
			WDims newDims = lParamToWDims(lParam);
			bool max = false;
			switch(wParam) {
				case SIZE_MINIMIZED:
					state_ |= ws_minimized;
					state_ &= ~ws_maximized;
					break;
				case SIZE_MAXIMIZED:
					max = true;
					state_ &= ~ws_minimized;
					state_ |= ws_maximized;
					break;
				case SIZE_RESTORED:
					state_ &= ~(ws_minimized | ws_maximized);
					break;
			}
            
			bool notChanged = true;
            auto resNewDims = findExec(this,cbResize,newDims);
            if( resNewDims ) {
                if( *resNewDims != newDims ) {
                	if( max ) {
                		ShowWindow(hWnd,SW_RESTORE);
                	}
            		setDims(*resNewDims);
            		notChanged = false;
                }
            }

            if( notChanged ) {
            	executeCallback(this, cbAfterResize, newDims);
            }
		} break;
        
		case WM_COMMAND: {
            if( lParam == 0 ) {
                for( auto & component : components ) {
                    auto result = component->processNotification(WM_COMMAND,HIWORD(wParam),LOWORD(wParam),lParam);
                    if( result ) return result;
                }
            } else
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
                if( executeCallback(this,cbUserEvent,toUserEvent(message,lParam)) ) {
                    return 0;
                }
			}
	}

	return optional<LRESULT>();
}

}
