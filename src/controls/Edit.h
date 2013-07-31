#ifndef EDIT_CONTROL_H_jfffffffff9329jdsfn
#define EDIT_CONTROL_H_jfffffffff9329jdsfn

#include "../Control.h"

namespace tk {

class Edit: public Control {
public:
    Edit() = default;
    Edit(Window & parent, ControlParams const & params = ControlParams());
    
	void setReadOnly(bool readOnly);
    
	using Control::getText;
	using Control::setText;

    using Control::onKey;
    using Control::onKeypress;
};

}


#endif
