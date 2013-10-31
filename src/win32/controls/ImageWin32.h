#ifndef IMAGE_WIN32_H_j3422224fdsfdsfs342432432332sa354h
#define IMAGE_WIN32_H_j3422224fdsfdsfs342432432332sa354h

#include "NativeControlWin32.h"

namespace tk {

class ImageImpl: public NativeControlImpl {
	scoped::Bitmap bitmap;
	bool autosize;
	WDims imageDims;
public:
	ImageImpl(HWND parentHwnd, ControlParams const & params);
    
    virtual ImageImpl * clone() const;

	void setImage(unsigned char const buffer[], void * header);

	void setAutosize(bool autosize);
private:
    void setImageBitmap();
};

}

#endif
