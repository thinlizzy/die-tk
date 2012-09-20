#ifndef BUTTON_WIN32_H_kkg921aznf3234829reiufd
#define BUTTON_WIN32_H_kkg921aznf3234829reiufd

#include "NativeControlWin32.h"
#include "../../controls/Button.h"

#pragma warning( push )
#pragma warning( disable : 4250 )

namespace tk {

class ButtonImpl: public NativeControlImpl, public Button {
	WDims getDimsByText(WDims dims);
public:
	ButtonImpl(HWND parent_hWnd, ControlParams const & params);

	virtual std::string getText() const;
	virtual void setText(std::string const & text);

	virtual void setDims(WDims dims);
};

}

#pragma warning( pop )

#endif
