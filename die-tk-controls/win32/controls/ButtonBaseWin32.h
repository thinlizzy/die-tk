#ifndef BUTTON_BASE_WIN32_H_fapsafpas3248f32bn324237fgj
#define BUTTON_BASE_WIN32_H_fapsafpas3248f32bn324237fgj

#include "../../../win32/die-tk-win32.h"

namespace tk {

class ButtonBaseImpl: public NativeControlImpl {
public:
	ButtonBaseImpl(HWND parentHwnd, ControlParams const & params, DWORD style);
    
    virtual ~ButtonBaseImpl();
    
    virtual optional<LRESULT> processNotification(UINT message, UINT notification, WPARAM wParam, LPARAM lParam);    
    
	HandleOperation onClick(HandleOperation callback);
};

}



#endif
