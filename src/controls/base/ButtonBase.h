#ifndef BUTTON_BASE_H_gjhrjh4543jfdsgjdsio23
#define BUTTON_BASE_H_gjhrjh4543jfdsgjdsio23

#include "../../Control.h"

namespace tk {

class ButtonBase: public Control {
protected:
    ButtonBase() = default;
public:
	using Control::getText;
	using Control::setText;
    HandleOperation onClick(HandleOperation callback);
};

}

#endif
