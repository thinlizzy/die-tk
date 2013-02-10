#include "ButtonBaseWin32.h"
#include "../ApplicationWin32.h"

namespace tk {

ButtonBaseImpl::ButtonBaseImpl(HWND parent_hWnd, ControlParams const & params, DWORD style):
    NativeControlImpl(parent_hWnd,params,"BUTTON",style)
{}
  
optional<LRESULT> ButtonBaseImpl::processNotification(UINT message, UINT notification, WPARAM wParam, LPARAM lParam)
{
    if( message == WM_COMMAND && notification == BN_CLICKED ) {
       if( findExec(globalAppImpl->onClick,shared_from_this()) ) return 0;
	}

    return NativeControlImpl::processNotification(message,notification,wParam,lParam);
}

}