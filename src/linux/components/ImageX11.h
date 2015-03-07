#ifndef IMAGEX11_H_DIE_TK_2015_02_01
#define IMAGEX11_H_DIE_TK_2015_02_01

#include "../../components/Image.h"
#include <memory>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

namespace tk {

namespace image {

class ImageX11: public Image {
	struct DestroyXImage {
		void operator()(XImage * imagePtr) const {
			if (imagePtr) XDestroyImage(imagePtr);
		}
	};
	std::unique_ptr<XImage,DestroyXImage> xImage;
public:
	explicit ImageX11(XImage * imagePtr);
    unsigned bpp() const override;
    WDims dims() const override;
    Canvas & beginDraw() override;
    Canvas & canvas() override;
    void endDraw() override;
    void drawInto(Canvas & canvas, Point dest) override;
    void drawInto(Canvas & canvas, Rect destrect) override;
};

}

}

#endif
