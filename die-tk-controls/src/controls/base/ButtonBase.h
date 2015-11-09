#ifndef BUTTON_BASE_H_gjhrjh4543jfdsgjdsio23
#define BUTTON_BASE_H_gjhrjh4543jfdsgjdsio23

#include "../../../../src/die-tk.h"

namespace tk {

class ButtonBase: public Control {
protected:
    ButtonBase() = default;
public:
    HandleOperation onClick(HandleOperation callback);
};

}

#endif
