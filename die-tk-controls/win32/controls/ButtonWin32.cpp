#include "ButtonWin32.h"

namespace tk {

ButtonImpl::ButtonImpl(HWND parentHwnd, ControlParams const & params):
	ButtonBaseImpl(parentHwnd,params,BS_PUSHBUTTON)
{
	if( ! params.text_.empty() ) {
		setText(params.text_);
		if( params.dims_.width == 0 || params.dims_.height == 0 ) {
			auto dims = getDimsByText(params.dims_);
			NativeControlImpl::setDims(dims);
		}
	}
}

ButtonImpl * ButtonImpl::clone() const
{
    return new ButtonImpl(getParentHandle(),getControlData());
}

WDims ButtonImpl::getDimsByText(WDims dims)
{
	// TODO: get values from current font
	int const width_padding = 8;
	int const height_padding = 6;

    auto size = getTextDims();

	if( dims.width == 0 ) {
		dims.width = size.cx + width_padding;
	}
	if( dims.height == 0 ) {
		dims.height = size.cy + height_padding;
	}
	return dims;
}

void ButtonImpl::setDims(WDims dims)
{
	if( dims.width == 0 || dims.height == 0 ) {
		dims = getDimsByText(dims);
	}
	NativeControlImpl::setDims(dims);
}

}
