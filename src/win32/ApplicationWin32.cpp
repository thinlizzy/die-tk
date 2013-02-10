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
			if( native->processMessage(msg.message,msg.wParam,msg.lParam) ) {
                TRACE_M("AppProc HANDLED -> CONTROL processMessage! hWnd = " << msg.hwnd << " message = " << windowsMessageToString(msg.message));
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
    // TODO move handleCommonCallbacks to NativeControlImpl processMessage
    if( auto controlImpl = findControlOrWindow(hWnd) ) {
        TRACE_M("WndProc! hWnd = " << hWnd << " message = " << windowsMessageToString(message));
        optional<LRESULT> result = controlImpl->processMessage(message,wParam,lParam);
        if( result ) {
            TRACE_M("WndProc HANDLED! hWnd = " << hWnd << " message = " << windowsMessageToString(message));
            return *result;
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
                windowImpl->processMessage(message,wParam,lParam);
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
	auto it = controlMap.find(hWnd); 
    return it == controlMap.end() ? nullControl : it->second;
}

std::shared_ptr<NativeControlImpl> ApplicationImpl::findControlOrWindow(HWND hWnd) {
    if( auto result = findWindow(hWnd) ) return result;
    return findControl(hWnd);
}

}
