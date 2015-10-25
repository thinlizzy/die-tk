#include "ResourceManager.h"
#include "../src/trace.h"

namespace {

std::shared_ptr<tk::WindowImpl> nullWindow;
std::shared_ptr<tk::NativeControlImpl> nullControl;

}

namespace tk {

ResourceManager resourceManager;

ResourceManager::ResourceManager():
    scrdc(NULL)
{
    cursors[int(Cursor::arrow)] = LoadCursor(NULL, IDC_ARROW);
    cursors[int(Cursor::wait)] = LoadCursor(NULL, IDC_WAIT);
    cursors[int(Cursor::smallwait)] = LoadCursor(NULL, IDC_APPSTARTING);
    cursors[int(Cursor::hand)] = LoadCursor(NULL, IDC_HAND);
    cursors[int(Cursor::edit)] = LoadCursor(NULL, IDC_IBEAM);
    cursors[int(Cursor::help)] = LoadCursor(NULL, IDC_HELP);
    cursors[int(Cursor::cross)] = LoadCursor(NULL, IDC_CROSS);
}

HDC ResourceManager::screenDC()
{
    return scrdc.hdc;
}

void ResourceManager::processMessages()
{
	MSG msg;
	while( PeekMessage(&msg,NULL,0,0,PM_REMOVE) != 0 ) {
		if( auto native = findControl(msg.hwnd) ) {
		    handleTopLevelWindowMessages(msg.hwnd,msg.message,msg.wParam,msg.lParam);
			TRACE_M("AppProc -> CONTROL! hWnd = " << msg.hwnd << " message = " << windowsMessageToString(msg.message));
			if( native->processMessage(msg.message,msg.wParam,msg.lParam) ) {
                TRACE_M("AppProc HANDLED -> CONTROL processMessage! hWnd = " << msg.hwnd << 
                        " message = " << windowsMessageToString(msg.message));
            }
		}

		TRACE_M("AppProc -> DISPATCHED! handle = " << msg.hwnd << " message = " << windowsMessageToString(msg.message));
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return resourceManager.WndProc(hWnd,message,wParam,lParam);
}

LRESULT ResourceManager::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    if( auto controlImpl = findControlOrWindow(hWnd) ) {
        TRACE_M("WndProc! hWnd = " << hWnd << " message = " << windowsMessageToString(message));
        auto result = controlImpl->processMessage(message,wParam,lParam);
        if( result ) {
            TRACE_M("WndProc HANDLED! hWnd = " << hWnd << " message = " << windowsMessageToString(message));
            return *result;
        }
    }
    
	TRACE_M("WndProc -> DEFAULT. handle = " << hWnd << " message = " << windowsMessageToString(message));
	return DefWindowProc(hWnd,message,wParam,lParam);
}

void ResourceManager::handleTopLevelWindowMessages(HWND hWnd, UINT message, WPARAM & wParam, LPARAM & lParam)
{
	switch( message ) {
		case WM_KEYDOWN:
		case WM_KEYUP:
		case WM_CHAR:
			TRACE_M("handleTopLevelWindowMessages -> child handle = " << hWnd << 
                    " message = " << windowsMessageToString(message));
		    if( auto windowImpl = getTopLevelWindow(hWnd) ) {
                TRACE_M("handleTopLevelWindowMessages -> PARENT WINDOW! hWnd = " << windowImpl->hWnd << 
                        " message = " << windowsMessageToString(message));
                windowImpl->processMessage(message,wParam,lParam);
		    }
        break;
	}
}

std::shared_ptr<NativeControlImpl> ResourceManager::getTopLevelWindow(HWND hWnd)
{
    auto parentHWnd = GetAncestor(hWnd,GA_ROOT);
    if( ! parentHWnd ) return nullWindow;
    return findWindow(parentHWnd);
}

void ResourceManager::registerWindow(std::shared_ptr<WindowImpl> const & window)
{
	windowMap[window->hWnd] = window;
}

void ResourceManager::unregisterWindow(std::shared_ptr<WindowImpl> const & window)
{
	windowMap.erase(window->hWnd);
}

void ResourceManager::registerControl(std::shared_ptr<NativeControlImpl> const & control)
{
	controlMap[control->hWnd] = control;
}

void ResourceManager::unregisterControl(std::shared_ptr<NativeControlImpl> const & control)
{
    controlMap.erase(control->hWnd);
}

std::shared_ptr<WindowImpl> ResourceManager::findWindow(HWND hWnd)
{
	auto it = windowMap.find(hWnd);
    if( it == windowMap.end() ) return nullWindow;
    
    if( auto spt = it->second.lock() ) return spt;
        
    windowMap.erase(it);
    return nullWindow;
}

std::shared_ptr<NativeControlImpl> ResourceManager::findControl(HWND hWnd)
{
	auto it = controlMap.find(hWnd); 
    return it == controlMap.end() ? nullControl : it->second;
}

std::shared_ptr<NativeControlImpl> ResourceManager::findControlOrWindow(HWND hWnd)
{
    if( auto result = findWindow(hWnd) ) return result;
    return findControl(hWnd);
}

}
