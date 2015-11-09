#ifndef CHECKBOX_WIN32_H_jjjjjj3428fh234r32vvasdf25
#define CHECKBOX_WIN32_H_jjjjjj3428fh234r32vvasdf25

#include "ButtonBaseWin32.h"
#include "../../src/controls/CheckBox.h"

namespace tk {

class CheckBoxImpl: public ButtonBaseImpl {
	bool autosize;
	WDims getDimsByText();
public:
	CheckBoxImpl(HWND parentHwnd, ControlParams const & params);

    virtual CheckBoxImpl * clone() const;

	virtual void setText(NativeString const & text);
    
	bool checked() const;
	void check(bool state);
	void setAutosize(bool autosize);
};

}


#endif
