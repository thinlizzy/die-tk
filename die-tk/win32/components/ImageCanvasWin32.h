#ifndef IMAGE_CANVAS_WIN_H_DIE_TK_2019_06_26
#define IMAGE_CANVAS_WIN_H_DIE_TK_2019_06_26

#include "../../components/ImageCanvas.h"

namespace tk {

class ImageCanvasWin: public ImageCanvas {
	tk::image::Ptr imageBuffer;
	tk::Canvas & canvas;
public:
	explicit ImageCanvasWin(tk::image::Ptr imageBuffer);
	~ImageCanvasWin() override;
private:
	tk::Canvas & imageCanvas() override;
	tk::image::Ptr finishAndCreateImage() override;
};

}

#endif
