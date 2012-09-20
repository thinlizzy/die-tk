#ifndef EDIT_CONTROL_H_jfffffffff9329jdsfn
#define EDIT_CONTROL_H_jfffffffff9329jdsfn

#include "Control.h"
#include <string>

namespace tk {

class Edit: public virtual Control {
public:
	virtual std::string getText() const = 0;
	virtual void setText(std::string const & text) = 0;

	virtual void setReadOnly(bool readOnly) = 0;
};

}


#endif
