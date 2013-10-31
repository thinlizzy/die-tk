#include "../../controls/Memo.h"
#include "MemoWin32.h"

#define CONTROL_IMPL MemoImpl
#include "ControlMacros.h"

namespace tk {

CONSTRUCTOR_IMPL(Memo)
CLONE_IMPL(Memo);
    
void Memo::setReadOnly(bool readOnly)
{
    IMPL.setReadOnly(readOnly);
}

void Memo::setTextAlign(HTextAlign alignment)
{
    IMPL.setTextAlign(alignment);
}

StringList & Memo::lines()
{
    return IMPL.lines();
}

void Memo::scrollTo(unsigned lines)
{
    IMPL.scrollTo(lines);
}

}
