#ifndef IMAGE_CANVAS_X11_H_DIE_TK_2019_06_27
#define IMAGE_CANVAS_X11_H_DIE_TK_2019_06_27

#include "../../components/ImageCanvas.h"

namespace tk {

class ImageCanvasX11: public ImageCanvas {
	tk::image::Ptr imageBuffer;
	tk::Canvas & canvas;
public:
	explicit ImageCanvasX11(tk::image::Ptr imageBuffer);
	// dtor?
private:
	tk::Canvas & imageCanvas() override;
	tk::image::Ptr finishAndCreateImage() override;
};

}

#endif
