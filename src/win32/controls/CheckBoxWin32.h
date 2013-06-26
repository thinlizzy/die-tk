#ifndef CHECKBOX_WIN32_H_jjjjjj3428fh234r32vvasdf25
#define CHECKBOX_WIN32_H_jjjjjj3428fh234r32vvasdf25

#include "ButtonBaseWin32.h"
#include "../../controls/CheckBox.h"

namespace tk {

class CheckBoxImpl: public ButtonBaseImpl {
	bool autosize;
	WDims getDimsByText();
public:
	CheckBoxImpl(Window & parent, ControlParams const & params);

	virtual void setText(die::NativeString const & text);
    
	bool checked() const;
	void check(bool state);
	void setAutosize(bool autosize);
};

}


#endif
