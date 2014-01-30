#include "ImageWin32.h"
#include "../ConvertersWin32.h"
#include "../ScopedObjects.h"
#include "../../util/make_unique.h"
#include "../components/ImageWin32.h"
#include "../../log.h"

namespace tk {

ImageImpl::ImageImpl(HWND parentHwnd, ControlParams const & params):
	NativeControlImpl(parentHwnd,params,L"STATIC",SS_BITMAP),
	autosize(params.autosize_)
{
}

ImageImpl * ImageImpl::clone() const
{
    auto result = make_unique<ImageImpl>(getParentHwnd(),getControlData().autosize(autosize));
    result->imageDims = imageDims;
    if( bitmap ) {
        auto dc = getDC();
        result->bitmap.reset(image::cloneBitmap(dc.hdc,bitmap.get()));
        result->setImageBitmap();
    }
    return result.release();
}

void ImageImpl::setImage(image::Ptr image)
{
	imageDims = image->dims();
	if( autosize ) {
		setDims(imageDims);
	}

    auto imgImpl = std::dynamic_pointer_cast<image::ImageImpl>(image);
	bitmap.reset(imgImpl->cloneHbitmap());
    setImageBitmap();
}

void ImageImpl::setImageBitmap()
{
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
