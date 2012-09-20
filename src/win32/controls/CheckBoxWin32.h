#ifndef CHECKBOX_WIN32_H_jjjjjj3428fh234r32vvasdf25
#define CHECKBOX_WIN32_H_jjjjjj3428fh234r32vvasdf25

#include "NativeControlWin32.h"
#include "../../controls/CheckBox.h"

namespace tk {

class CheckBoxImpl: public NativeControlImpl, public CheckBox {
	bool autosize;
	WDims getDimsByText();
public:
	CheckBoxImpl(HWND parent_hWnd, ControlParams const & params);

	virtual std::string getText() const;
	virtual void setText(std::string const & text);
	virtual bool checked() const;
	virtual void check(bool state);
	virtual void setAutosize(bool autosize);
};

}


#endif
