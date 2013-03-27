#ifndef IMAGE_WIN32_H_j3422224fdsfdsfs342432432332sa354h
#define IMAGE_WIN32_H_j3422224fdsfdsfs342432432332sa354h

#include "NativeControlWin32.h"

namespace tk {

class ImageImpl: public NativeControlImpl {
	scoped::Bitmap bitmap;
	bool autosize;
	WDims imageDims;
public:
	ImageImpl(Window & parent, ControlParams const & params);

	void setImage(unsigned char const buffer[], void * header);

	void setAutosize(bool autosize);
};

}

#endif
