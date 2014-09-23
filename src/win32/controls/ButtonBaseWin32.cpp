#include "ButtonBaseWin32.h"
#include "../../CallbackUtils.h"

namespace tk {

CallbackMap<ButtonBaseImpl *, HandleOperation> cbClick;

ButtonBaseImpl::ButtonBaseImpl(HWND parentHwnd, ControlParams const & params, DWORD style):
    NativeControlImpl(parentHwnd,params,L"BUTTON",style)
{}

ButtonBaseImpl::~ButtonBaseImpl()
{
    removeFromCb(this,cbClick);
}

optional<LRESULT> ButtonBaseImpl::processNotification(UINT message, UINT notification, WPARAM wParam, LPARAM lParam)
{
    if( message == WM_COMMAND && notification == BN_CLICKED ) {
       if( executeCallback(this,cbClick) ) return 0;
	}

    return NativeControlImpl::processNotification(message,notification,wParam,lParam);
}

HandleOperation ButtonBaseImpl::onClick(HandleOperation callback)
{
    return setCallback(this,cbClick,callback);
}


}