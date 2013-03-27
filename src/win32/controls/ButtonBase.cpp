#include "../../controls/base/ButtonBase.h"
#include "ButtonBaseWin32.h"

#define CONTROL_IMPL ButtonBaseImpl
#include "ControlMacros.h"

namespace tk {
    
HandleOperation ButtonBase::onClick(HandleOperation callback)
{
    return IMPL.onClick(callback);
}
    
}
