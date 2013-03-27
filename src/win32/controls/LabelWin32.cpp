#include "LabelWin32.h"

namespace tk {

LabelImpl::LabelImpl(Window & parent, ControlParams const & params):
	NativeControlImpl(parent,params,"STATIC",SS_CENTER),
	autosize(params.autosize_)
{
	setText(params.text_);
    if( autosize ) {
        doAutosize();
    }
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
