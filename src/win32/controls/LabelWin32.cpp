#include "LabelWin32.h"

namespace tk {

LabelImpl::LabelImpl(HWND parent_hWnd, ControlParams const & params):
	NativeControlImpl(parent_hWnd,params,"STATIC",SS_CENTER),
	autosize(params.autosize_)
{
	setText(params.text_);
    if( autosize ) {
        doAutosize();
    }
}

std::string LabelImpl::getText() const
{
    return NativeControlImpl::getText();
}

void LabelImpl::setText(std::string const & text)
{
    NativeControlImpl::setText(text);
}

void LabelImpl::setAutosize(bool autosize)
{
   	bool redim = autosize && ! this->autosize;
	this->autosize = autosize;
    if( redim ) {
        doAutosize();
    }
}

void LabelImpl::doAutosize()
{
	// TODO: make them properties
	int const width_padding = 4;
	int const height_padding = 2;

    auto size = getTextDims();
    setDims(WDims(size.cx + width_padding, size.cy + height_padding));
}


}
