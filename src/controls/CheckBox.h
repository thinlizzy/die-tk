#ifndef CHECKBOX_H_fk3492gjl50fh27g
#define CHECKBOX_H_fk3492gjl50fh27g

#include "Control.h"
#include <string>

namespace tk {

class CheckBox: public virtual Control {
public:
	virtual std::string getText() const = 0;
	virtual void setText(std::string const & text) = 0;
	virtual bool checked() const = 0;
	virtual void check(bool state) = 0;
	virtual void setAutosize(bool autosize) = 0;
};

}

#endif
