#include "EditWin32.h"

namespace tk {

EditImpl::EditImpl(Window & parent, ControlParams const & params):
	NativeControlImpl(parent,params,"EDIT",0)
{
	if( ! params.text_.empty() ) {
		setText(params.text_);
	}
}

void EditImpl::setReadOnly(bool readOnly)
{
    PostMessage(hWnd,EM_SETREADONLY,readOnly,0);
}

}
