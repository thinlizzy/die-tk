#include "ButtonBaseWin32.h"
#include "../ApplicationWin32.h"

namespace tk {

ButtonBaseImpl::ButtonBaseImpl(HWND parent_hWnd, ControlParams const & params, char const classname[], DWORD style):
    NativeControlImpl(parent_hWnd,params,classname,style)
{}
  
bool ButtonBaseImpl::processNotification(UINT message, UINT notification, WPARAM wParam, LPARAM lParam)
{
    if( message == WM_COMMAND && notification == BN_CLICKED ) {
        auto it = globalAppImpl->onClick.find(shared_from_this());
        if( it == globalAppImpl->onClick.end() ) return false;
        
        auto & on_click = it->second;
        on_click();
        return true;
	}

    return NativeControlImpl::processNotification(message,notification,wParam,lParam);
}

}