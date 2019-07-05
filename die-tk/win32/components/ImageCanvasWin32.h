#ifndef IMAGE_CANVAS_WIN_H_DIE_TK_2019_06_26
#define IMAGE_CANVAS_WIN_H_DIE_TK_2019_06_26

#include "../../components/ImageCanvas.h"
#include "../CanvasImplWin32.h"

namespace tk {

class ImageCanvasWin: public ImageCanvas {
	tk::image::Ptr imageBuffer;
	tk::CanvasImpl & canvasImpl;
public:
	explicit ImageCanvasWin(tk::image::Ptr imageBuffer);
	~ImageCanvasWin() override;
private:
	tk::Canvas & imageCanvas() override;
	tk::image::Ptr finishAndCreateImage() override;

	void drawImage(tk::image::Ptr const & image, tk::Point pos) override;
	void drawImage(tk::image::Ptr const & image, Rect destrect) override;
	void copyRectImage(tk::image::Ptr const & image, Rect srcrect, Point dest) override;
};

}

#endif
