#ifndef CHECKBOX_H_fk3492gjl50fh27g
#define CHECKBOX_H_fk3492gjl50fh27g

#include "base/ButtonBase.h"

namespace tk {

class CheckBox: public ButtonBase {
public:
    CheckBox() = default;
    CheckBox(Window & parent, ControlParams const & params = ControlParams());    
    CheckBox clone() const;
	bool checked() const;
	void check(bool state);
	void setAutosize(bool autosize);
};

}

#endif
