#include "../../controls/Memo.h"
#include "MemoWin32.h"

#define CONTROL_IMPL MemoImpl
#include "ControlMacros.h"

namespace tk {

CONSTRUCTOR_IMPL(Memo)
    
void Memo::setReadOnly(bool readOnly)
{
    IMPL.setReadOnly(readOnly);
}

}
