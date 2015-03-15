#ifndef IMAGEX11_H_DIE_TK_2015_02_01
#define IMAGEX11_H_DIE_TK_2015_02_01

#include "../../components/Image.h"
#include <memory>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

namespace tk {

namespace image {

struct DestroyXImage {
	void operator()(XImage * imagePtr) const {
		if (imagePtr) XDestroyImage(imagePtr);
	}
};

using xImagePtr = std::unique_ptr<XImage,DestroyXImage>;

class ImageX11: public Image {
	xImagePtr xImage;
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

class ImageX11Transparent: public ImageX11 {
	std::unique_ptr<unsigned char[]> transparentBuffer;
	Pixmap transparentMask;
public:
	ImageX11Transparent(XImage * imagePtr);
	~ImageX11Transparent();

    void drawInto(Canvas & canvas, Point dest) override;
    void drawInto(Canvas & canvas, Rect destrect) override;
};

// result image will own buffer
Ptr createNativeBGRA(WDims dims, char * buffer);
Ptr createTransparentBGRA(WDims dims, char * buffer);

}

}

#endif
