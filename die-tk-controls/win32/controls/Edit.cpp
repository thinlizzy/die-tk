#include "../../src/controls/Edit.h"
#include "EditWin32.h"

#define CONTROL_IMPL EditImpl
#include "ControlMacros.h"

namespace tk {

CONSTRUCTOR_IMPL(Edit);
CLONE_IMPL(Edit);
    
void Edit::setReadOnly(bool readOnly)
{
    IMPL.setReadOnly(readOnly);
}

void Edit::setTextAlign(HTextAlign alignment)
{
    IMPL.setTextAlign(alignment);
}

bool Edit::readOnly() const
{
    return IMPL.readOnly();
}
    
}
