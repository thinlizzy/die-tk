#ifndef BUTTON_BASE_WIN32_H_fapsafpas3248f32bn324237fgj
#define BUTTON_BASE_WIN32_H_fapsafpas3248f32bn324237fgj

#include "NativeControlWin32.h"

namespace tk {

class ButtonBaseImpl: public NativeControlImpl {
public:
	ButtonBaseImpl(HWND parent_hWnd, ControlParams const & params, DWORD style);
    
    virtual optional<LRESULT> processNotification(UINT message, UINT notification, WPARAM wParam, LPARAM lParam);    
};

}



#endif
