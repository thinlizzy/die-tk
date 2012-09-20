#include "EditWin32.h"

namespace tk {

EditImpl::EditImpl(HWND parent_hWnd, ControlParams const & params):
	NativeControlImpl(parent_hWnd,params,"EDIT",0)
{
	if( ! params.text_.empty() ) {
		setText(params.text_);
	}
}

std::string EditImpl::getText() const
{
    return NativeControlImpl::getText();
}

void EditImpl::setText(std::string const & text)
{
    NativeControlImpl::setText(text);
}

void EditImpl::setReadOnly(bool readOnly)
{
    PostMessage(hWnd,EM_SETREADONLY,readOnly,0);
}

}
