#ifndef LABEL_H_hfjds34230pp2b4jugf
#define LABEL_H_hfjds34230pp2b4jugf

#include "../Control.h"

namespace tk {

class Label: public Control {
public:
    Label() = default;
    Label(Window & parent, ControlParams const & params = ControlParams());
    
	void setAutosize(bool autosize);
    
	using Control::getText;
	using Control::setText;

    using Control::onMouse;
};

}


#endif
