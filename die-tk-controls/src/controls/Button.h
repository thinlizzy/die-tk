#ifndef BUTTON_H_gjhrjh4543jfdsgjdsio23
#define BUTTON_H_gjhrjh4543jfdsgjdsio23

#include "base/ButtonBase.h"

namespace tk {

class Button: public ButtonBase {
public:
    Button() = default;
    Button(Window & parent, ControlParams const & params = ControlParams());
    Button clone() const;
};

}

#endif
