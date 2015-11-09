#ifndef BUTTON_WIN32_H_kkg921aznf3234829reiufd
#define BUTTON_WIN32_H_kkg921aznf3234829reiufd

#include "ButtonBaseWin32.h"
#include "../../src/controls/Button.h"

namespace tk {

class ButtonImpl: public ButtonBaseImpl {
	WDims getDimsByText(WDims dims);
public:
	ButtonImpl(HWND parentHwnd, ControlParams const & params);
    
    virtual ButtonImpl * clone() const;

	virtual void setDims(WDims dims);
};

}

#endif
