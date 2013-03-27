#ifndef BUTTON_WIN32_H_kkg921aznf3234829reiufd
#define BUTTON_WIN32_H_kkg921aznf3234829reiufd

#include "ButtonBaseWin32.h"
#include "../../controls/Button.h"

namespace tk {

class ButtonImpl: public ButtonBaseImpl {
	WDims getDimsByText(WDims dims);
public:
	ButtonImpl(Window & parent, ControlParams const & params);

	virtual void setDims(WDims dims);
};

}

#endif
