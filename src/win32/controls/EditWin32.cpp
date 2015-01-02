#include "EditWin32.h"
#include <memory>
#include <stdexcept>

namespace tk {

EditImpl::EditImpl(HWND parentHwnd, ControlParams const & params):
    EditImpl(parentHwnd, params, 0)
{}

EditImpl::EditImpl(HWND parentHwnd, ControlParams const & params, DWORD style):
	NativeControlImpl(parentHwnd,params,L"EDIT",style)
{
	if( ! params.text_.empty() ) {
		setText(params.text_);
	}
}

EditImpl * EditImpl::clone() const
{
    auto result = std::make_unique<EditImpl>(getParentHandle(),getControlData());
    cloneInto(*result);
    return result.release();
}

void EditImpl::cloneInto(EditImpl & result) const
{
    if( readOnly() ) {
        result.setReadOnly(true);
    }
}

void EditImpl::setReadOnly(bool readOnly)
{
    PostMessage(hWnd,EM_SETREADONLY,readOnly,0);
}

DWORD alignmentToEditStyle(HTextAlign alignment)
{
    switch(alignment) {
        case HTextAlign::left: return ES_LEFT;
        case HTextAlign::right: return ES_RIGHT;
        case HTextAlign::center: return ES_CENTER;
    }
    throw std::invalid_argument("invalid horizontal alignment");
}

void EditImpl::setTextAlign(HTextAlign alignment)
{
    DWORD oldStyle = GetWindowLong(hWnd,GWL_STYLE);
    DWORD newStyle = (oldStyle & ~(ES_LEFT | ES_CENTER | ES_RIGHT)) | alignmentToEditStyle(alignment);
    SetWindowLong(hWnd,GWL_STYLE,newStyle);
    SetWindowPos(hWnd,0,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
}

bool EditImpl::readOnly() const
{
    auto style = GetWindowLong(hWnd,GWL_STYLE);
    return (style & ES_READONLY) == ES_READONLY;
}

}
