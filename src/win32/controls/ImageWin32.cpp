#include "ImageWin32.h"
#include "../ConvertersWin32.h"
#include "../ScopedObjects.h"
#include "../../util/make_unique.h"

namespace tk {

ImageImpl::ImageImpl(HWND parentHwnd, ControlParams const & params):
	NativeControlImpl(parentHwnd,params,L"STATIC",SS_BITMAP),
	autosize(params.autosize_)
{
}

// TODO test this (I don't like the NULLs. should be screen DC)
HBITMAP copyBitmap(HBITMAP srchbitmap, HDC windowDC)
{
    BITMAP header;
    GetObject(srchbitmap,sizeof(BITMAP),&header);
    
    scoped::CompatibleDC srccdc = CreateCompatibleDC(NULL);
    auto lastObject = SelectObject(srccdc.get(),srchbitmap);
    
    scoped::CompatibleDC destcdc = CreateCompatibleDC(NULL);
    HBITMAP result = CreateCompatibleBitmap( windowDC, header.bmWidth, header.bmHeight );
    BitBlt( destcdc.get(), 0, 0, header.bmWidth, header.bmHeight, srccdc.get(), 0, 0, SRCCOPY );
    
    SelectObject(srccdc.get(),lastObject);
    
    return result;
}

ImageImpl * ImageImpl::clone() const
{
    auto result = make_unique<ImageImpl>(getParentHwnd(),getControlData().autosize(autosize));
    result->imageDims = imageDims;
    if( bitmap ) {
        auto dc = getDC();
        result->bitmap.reset(copyBitmap(bitmap.get(),dc.hdc));
        result->setImageBitmap();
    }
    return result.release();
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
