#ifndef IMAGE_WIN32_H_j3422224fdsfdsfs342432432332sa354h
#define IMAGE_WIN32_H_j3422224fdsfdsfs342432432332sa354h

#include "PaintBoxWin32.h"
#include "../../src/Canvas.h"
#include "../../src/components/Image.h"

namespace tk {

class ImageImpl: public PaintBoxImpl {
	bool autosize;
	image::Ptr image;
public:
	ImageImpl(HWND parentHwnd, ControlParams const & params);
    
    ImageImpl * clone() const override;

	void setImage(image::Ptr image);

	void setAutosize(bool autosize);
    
    Canvas & beginDraw();
    void endDraw();
private:
    bool hasImage() const;
    void handlePaint(Canvas & canvas, Rect rect);
};

}

#endif
