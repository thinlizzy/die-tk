#include "../../src/controls/Button.h"
#include "ButtonWin32.h"

#define CONTROL_IMPL ButtonImpl
#include "ControlMacros.h"

namespace tk {
    
CONSTRUCTOR_IMPL(Button);
CLONE_IMPL(Button);

}
