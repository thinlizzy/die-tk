#include "ApplicationWin32.h"
#include "../trace.h"

namespace {

std::shared_ptr<tk::WindowImpl> nullWindow;
std::shared_ptr<tk::NativeControlImpl> nullControl;

}

namespace tk {

std::shared_ptr<ApplicationImpl> globalAppImpl(new ApplicationImpl);

ApplicationImpl::ApplicationImpl()
{
    cursors[cur_arrow] = LoadCursor(NULL, IDC_ARROW);
    cursors[cur_wait] = LoadCursor(NULL, IDC_WAIT);
    cursors[cur_smallwait] = LoadCursor(NULL, IDC_APPSTARTING);
    cursors[cur_hand] = LoadCursor(NULL, IDC_HAND);
    cursors[cur_edit] = LoadCursor(NULL, IDC_IBEAM);
    cursors[cur_help] = LoadCursor(NULL, IDC_HELP);
    cursors[cur_cross] = LoadCursor(NULL, IDC_CROSS);
}

void ApplicationImpl::processMessages()
{
	MSG msg;
	while( PeekMessage(&msg,NULL,0,0,PM_REMOVE) != 0 ) {
		if( auto native = findControl(msg.hwnd) ) {
		    handleTopLevelWindowMessages(msg.hwnd,msg.message,msg.wParam,msg.lParam);
			TRACE_M("AppProc -> CONTROL! hWnd = " << msg.hwnd << " message = " << windowsMessageToString(msg.message));
			native->processMessage(msg.message,msg.lParam,msg.wParam);
			if( handleControlCallbacks(msg.message,native,msg.wParam,msg.lParam) ) {
                TRACE_M("AppProc HANDLED -> CONTROL! hWnd = " << msg.hwnd << " message = " << windowsMessageToString(msg.message));
			}
		}

		TRACE_M("AppProc -> DISPATCHED! handle = " << msg.hwnd << " message = " << windowsMessageToString(msg.message));
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return globalAppImpl->WndProc(hWnd,message,wParam,lParam);
}

LRESULT ApplicationImpl::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if( auto windowImpl = findWindow(hWnd) ) {
		TRACE_M("WndProc -> WINDOW! hWnd = " << hWnd << " message = " << windowsMessageToString(message));
		int handled = 0;
		if( handleControlCallbacks(message,windowImpl,wParam,lParam) ) handled = 1;
		else if( handleWindowCallbacks(message,windowImpl,wParam,lParam) ) handled = 1;
		else if( windowImpl->processMessage(message,wParam,lParam) ) handled = 2;
		if( handled ) {
		    TRACE_M("WndProc HANDLED " << handled << " -> WINDOW! hWnd = " << hWnd << " message = " << windowsMessageToString(message));
		    return 0;
		}
	} else {
		if( auto controlImpl = findControl(hWnd) ) {
			TRACE_M("WndProc -> CONTROL! handle = " << hWnd << " message = " << windowsMessageToString(message));
            int handled = 0;
			if( handleControlCallbacks(message,controlImpl,wParam,lParam) ) handled = 1;
			else if( controlImpl->processMessage(message,wParam,lParam) ) handled = 2;
            if( handled ) {
                TRACE_M("WndProc HANDLED " << handled << " -> CONTROL! hWnd = " << hWnd << " message = " << windowsMessageToString(message));
                return 0;
            }
		}
	}

	TRACE_M("WndProc -> DEFAULT. handle = " << hWnd << " message = " << windowsMessageToString(message));
	return DefWindowProc(hWnd,message,wParam,lParam);
}

void ApplicationImpl::handleTopLevelWindowMessages(HWND hWnd, UINT message, WPARAM & wParam, LPARAM & lParam)
{
	switch( message ) {
		case WM_KEYDOWN:
		case WM_KEYUP:
		case WM_CHAR:
			TRACE_M("handleTopLevelWindowMessages -> child handle = " << hWnd << " message = " << windowsMessageToString(message));
		    if( auto windowImpl = getTopLevelWindow(hWnd) ) {
                TRACE_M("handleTopLevelWindowMessages -> PARENT WINDOW! hWnd = " << windowImpl->hWnd << " message = " << windowsMessageToString(message));
		        handleControlCallbacks(message,windowImpl,wParam,lParam);
		    }
        break;
	}
}

std::shared_ptr<NativeControlImpl> ApplicationImpl::getTopLevelWindow(HWND hWnd)
{
    auto parentHWnd = GetAncestor(hWnd,GA_ROOT);
    if( ! parentHWnd ) return nullWindow;
    return findWindow(parentHWnd);
}

#define GETCB(mapname,varname) \
			auto it = mapname.find(control); \
			if( it == mapname.end() ) return false; \
			auto & varname = it->second;

bool ApplicationImpl::handleControlCallbacks(UINT message, std::shared_ptr<NativeControlImpl> control, WPARAM & wParam, LPARAM & lParam)
{
	switch( message ) {
		case WM_PAINT: {
			GETCB(onPaint,on_paint);
			scoped::PaintSection sps(control->hWnd);
			CanvasImpl canvas(sps.ps.hdc);
			auto rect = convertRect(sps.ps.rcPaint);
			on_paint(canvas,rect);
			return true;
			} break;

		case WM_KEYDOWN:
		case WM_KEYUP:{
			GETCB(onKey,on_key);
			auto keyEvent = toKeyEvent(message,wParam);
			WindowKey key = on_key(keyEvent);
			if( key == k_NONE ) return true;
			wParam = toWindowsKey(key);
			} break;

		case WM_CHAR:{
			GETCB(onKeypress,on_keypress);
			char key = on_keypress(wParam);
			if( key == 0 ) return true;
			wParam = key;
			} break;

		case WM_LBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_RBUTTONDOWN:
		case WM_RBUTTONUP:
		case WM_MBUTTONDOWN:
		case WM_MBUTTONUP:
		case WM_MOUSEMOVE:{
			GETCB(onMouse,on_mouse);
			auto p = lParamToPoint(lParam);
			auto mouseEvent = toMouseEvent(message,wParam);
			on_mouse(mouseEvent,p);
			return true;
			} break;
	}

	return false;
}

bool ApplicationImpl::handleWindowCallbacks(UINT message, std::shared_ptr<WindowImpl> window, WPARAM & wParam, LPARAM & lParam)
{
#define GETCBW(mapname,varname) \
			auto it = mapname.find(window); \
			if( it == mapname.end() ) return false; \
			auto & varname = it->second;

	switch( message ) {
		case WM_CLOSE: {
			GETCBW(onClose,on_close);
			auto canClose = on_close();
			return ! canClose;
			} break;

		case WM_SIZE: {
			GETCBW(onResize,on_resize);
			WDims newDims = lParamToWDims(lParam);
			newDims = on_resize(newDims);				// TODO verify the need of doing on_minimize and on_maximize callbacks also
			lParam = WDimsToLParam(newDims);
			} break;

		case WM_COMMAND: {
			if( auto control = findControl(HWND(lParam)) ) {
				auto notification = HIWORD(wParam);
				switch(notification) {
					case BN_CLICKED:{
						GETCB(onClick,on_click);
						on_click();
						return true;
					}
					case CBN_SELCHANGE:{
						GETCB(onChange,on_change);
						on_change();
						return true;
					}
				}
			}
			} break;

		default:
			if( message >= WM_USER ) {
				GETCBW(onUserEvent,on_userEvent)
				on_userEvent(toUserEvent(message,lParam));
				return true;
			}
	}

	return false;
}

void ApplicationImpl::registerWindow(std::shared_ptr<WindowImpl> window) {
	windowMap[window->hWnd] = window;
}

void ApplicationImpl::registerControl(std::shared_ptr<NativeControlImpl> control) {
	controlMap[control->hWnd] = control;
}

std::shared_ptr<WindowImpl> ApplicationImpl::findWindow(HWND hWnd) {
	auto it = windowMap.find(hWnd);
	return it == windowMap.end() ? nullWindow : it->second;
}

std::shared_ptr<NativeControlImpl> ApplicationImpl::findControl(HWND hWnd) {
	auto it = controlMap.find(hWnd); return it == controlMap.end() ? nullControl : it->second;
}


}
