#include "../WindowRef.h"
#include "WindowImplWin32.h"

namespace tk {

WindowRef::WindowRef(std::shared_ptr<WindowImpl> impl):
    Window(impl)
{
}

WindowRef::WindowRef(WindowRef const & wr)
{
    this->impl = wr.impl;    
}

WindowRef & WindowRef::operator=(WindowRef const & wr)
{
    this->impl = wr.impl;
    return *this;
}

}
