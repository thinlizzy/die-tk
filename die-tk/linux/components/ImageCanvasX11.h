#ifndef IMAGE_CANVAS_X11_H_DIE_TK_2019_06_27
#define IMAGE_CANVAS_X11_H_DIE_TK_2019_06_27

#include "../../components/ImageCanvas.h"
#include "../CanvasX11.h"

namespace tk {

class ImageCanvasX11: public ImageCanvas {
	tk::image::Ptr imageBuffer;
	tk::CanvasX11 & canvas;
public:
	explicit ImageCanvasX11(tk::image::Ptr imageBuffer);
	// dtor?
private:
	tk::Canvas & imageCanvas() override;
	tk::image::Ptr finishAndCreateImage() override;

	void drawImage(tk::image::Ptr const & image, tk::Point pos) override;
	void drawImage(tk::image::Ptr const & image, Rect destrect) override;
	void copyRectImage(tk::image::Ptr const & image, Rect srcrect, Point dest) override;
};

}

#endif
