#include "../../controls/Edit.h"
#include "EditWin32.h"

#define CONTROL_IMPL EditImpl
#include "ControlMacros.h"

namespace tk {

CONSTRUCTOR_IMPL(Edit);
    
void Edit::setReadOnly(bool readOnly)
{
    IMPL.setReadOnly(readOnly);
}
    
}
