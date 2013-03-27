#include "../../controls/CheckBox.h"
#include "CheckBoxWin32.h"

#define CONTROL_IMPL CheckBoxImpl
#include "ControlMacros.h"

namespace tk {

CONSTRUCTOR_IMPL(CheckBox);
    
bool CheckBox::checked() const
{
    return IMPL.checked();
}

void CheckBox::check(bool state)
{
    IMPL.check(state);
}

void CheckBox::setAutosize(bool autosize)
{
    IMPL.setAutosize(autosize);
}

}
