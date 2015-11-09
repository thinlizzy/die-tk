#include "../../src/controls/Label.h"
#include "LabelWin32.h"

#define CONTROL_IMPL LabelImpl
#include "ControlMacros.h"

namespace tk {

CONSTRUCTOR_IMPL(Label);
CLONE_IMPL(Label);

void Label::setAutosize(bool autosize)
{
    IMPL.setAutosize(autosize);    
}

}
