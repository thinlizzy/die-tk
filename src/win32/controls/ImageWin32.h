#ifndef IMAGE_WIN32_H_j3422224fdsfdsfs342432432332sa354h
#define IMAGE_WIN32_H_j3422224fdsfdsfs342432432332sa354h

#include "NativeControlWin32.h"
#include "../../controls/Image.h"

#pragma warning( push )
#pragma warning( disable : 4250 )

namespace tk {

class ImageImpl: public NativeControlImpl, public Image {
	scoped::Bitmap bitmap;
	bool autosize;
	WDims imageDims;
public:
	ImageImpl(HWND parent_hWnd, ControlParams const & params);

	virtual void setImage(unsigned char const buffer[], void * header);

	virtual void setAutosize(bool autosize);
};

}

#pragma warning( pop )

#endif
