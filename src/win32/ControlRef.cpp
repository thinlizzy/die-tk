#include "../ControlRef.h"
#include "controls/NativeControlWin32.h"

namespace tk {

ControlRef::ControlRef(Control const & control)
{
    this->impl = control.impl;
}

ControlRef::ControlRef(ControlRef const & cr)
{
    this->impl = cr.impl;    
}

ControlRef & ControlRef::operator=(ControlRef const & cr)
{
    this->impl = cr.impl;
    return *this;
}

}
