#include "CheckBoxWin32.h"
#include <algorithm>
#include <Windowsx.h>
#include <memory>

namespace tk {

CheckBoxImpl::CheckBoxImpl(HWND parentHwnd, ControlParams const & params):
	ButtonBaseImpl(parentHwnd,params,BS_AUTOCHECKBOX),
	autosize(params.autosize_)
{
	setText(params.text_);
}

CheckBoxImpl * CheckBoxImpl::clone() const
{
    auto result = std::make_unique<CheckBoxImpl>(getParentHandle(),getControlData().autosize(autosize));
    if( checked() ) {
        result->check(true);
    }
    return result.release();
}

void CheckBoxImpl::setText(die::NativeString const & text)
{
    NativeControlImpl::setText(text);
    if( autosize ) {
        setDims(getDimsByText());
    }
}

bool CheckBoxImpl::checked() const
{
    return Button_GetCheck(hWnd) == BST_CHECKED;
}

void CheckBoxImpl::check(bool state)
{
    Button_SetCheck(hWnd,state ? BST_CHECKED : BST_UNCHECKED);
}

void CheckBoxImpl::setAutosize(bool autosize)
{
    if( autosize && ! this->autosize ) {
        setDims(getDimsByText());
    }
   	this->autosize = autosize;
}

WDims CheckBoxImpl::getDimsByText()
{
	// TODO: get values from current font
	int const width_padding = 20;
	int const height_padding = 2;
	int const min_height = 15;

    auto size = getTextDims();
    return WDims(size.cx + width_padding,std::max<int>(size.cy + height_padding,min_height));
}

}
