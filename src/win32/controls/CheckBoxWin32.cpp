#include "CheckBoxWin32.h"
#include <algorithm>
#include <Windowsx.h>

namespace tk {

CheckBoxImpl::CheckBoxImpl(HWND parent_hWnd, ControlParams const & params):
	NativeControlImpl(parent_hWnd,params,"BUTTON",BS_AUTOCHECKBOX),
	autosize(params.autosize_)
{
	setText(params.text_);
}

std::string CheckBoxImpl::getText() const
{
    return NativeControlImpl::getText();
}

void CheckBoxImpl::setText(std::string const & text)
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
	int const width_padding = 10;
	int const height_padding = 2;
	int const min_height = 15;

    auto size = getTextDims();
    return WDims(size.cx + width_padding,std::max<int>(size.cy + height_padding,min_height));
}

}
