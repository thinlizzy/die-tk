#ifndef IMAGEX11_H_DIE_TK_2015_02_01
#define IMAGEX11_H_DIE_TK_2015_02_01

#include <memory>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include "../ScopedX11.h"
#include "../CanvasX11.h"
#include "../../components/Image.h"

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
	scoped::Pixmap drawingArea;
	CanvasX11 drawingCanvas;
public:
	explicit ImageX11(XImage * imagePtr);
	unsigned bpp() const override;
	WDims dims() const override;
	CanvasX11 & beginDraw() override;
	CanvasX11 & canvas() override;
	void endDraw() override;
	void drawInto(CanvasX11 & canvas, Point dest) override;
	void drawInto(CanvasX11 & canvas, Rect destrect) override;
	void copyRectInto(CanvasX11 & canvas, Rect srcrect, Point dest) override;
};

class ImageX11Transparent: public ImageX11 {
	scoped::Pixmap transparentMask;
public:
	ImageX11Transparent(XImage * imagePtr);
	ImageX11Transparent(XImage * imagePtr, std::vector<bool> const & transparentMask);

	void drawInto(CanvasX11 & canvas, Point dest) override;
	void drawInto(CanvasX11 & canvas, Rect destrect) override;
	void copyRectInto(CanvasX11 & canvas, Rect srcrect, Point dest) override;
};

// result image will own buffer
Ptr createNativeBGRA(WDims dims, char * buffer);
Ptr createTransparentBGRA(WDims dims, char * buffer);
Ptr createTransparentBGRA(WDims dims, std::vector<bool> const & transparentMask, char * buffer);

}

}

#endif
