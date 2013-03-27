#include "ImageWin32.h"
#include "../ConvertersWin32.h"
#include "../ScopedObjects.h"

namespace tk {

ImageImpl::ImageImpl(Window & parent, ControlParams const & params):
	NativeControlImpl(parent,params,"STATIC",SS_BITMAP),
	autosize(params.autosize_)
{
}

void ImageImpl::setImage(unsigned char const buffer[], void * header)
{
	auto info = reinterpret_cast<BITMAPINFO *>(header);
	imageDims = WDims(info->bmiHeader.biWidth,abs(info->bmiHeader.biHeight));
	if( autosize ) {
		setDims(imageDims);
	}

	scoped::DC hdc_scr(NULL);
	bitmap.reset(CreateDIBitmap(hdc_scr.hdc, &info->bmiHeader,
        CBM_INIT, buffer,
        info,
        DIB_RGB_COLORS));
	SendMessage(hWnd, STM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)bitmap.get());
}

void ImageImpl::setAutosize(bool autosize)
{
	bool redim = autosize && ! this->autosize;
	this->autosize = autosize;
	if( redim && bitmap.get() != 0 ) {
		setDims(imageDims);
	}
}

}
