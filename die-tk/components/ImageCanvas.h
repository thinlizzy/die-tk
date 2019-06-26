#ifndef IMAGE_CANVAS_H_DIE_TK_2019_06_25
#define IMAGE_CANVAS_H_DIE_TK_2019_06_25

#include "../Canvas.h"
#include "Image.h"

namespace tk {

class ImageCanvas: public Canvas {
public:
	static std::shared_ptr<ImageCanvas> create(tk::WDims dims, bool transparent = false);

	void translate(Point p) override;
	void clearTranslate() override;
	void addClipRect(Rect const & openrect) override;
	void clearClipping() override;

	void plot(Point p, RGBColor const & color) override;
	void drawLine(Point p1, Point p2, Pen const & pen) override;
	void drawPoly(Points const & polygon, Pen const & pen) override;

	void rectangle(Rect const & rect, Pen const & pen) override;
	void fillRect(Rect const & openrect, Brush const & brush) override;
	void roundRect(Rect const & openrect, WDims ellipseDims, Pen const & pen, Brush const & brush) override;

	void drawText(Point p, NativeString const & text, RGBColor const & color) override;
	void drawText(Point p, NativeString const & text, RGBColor const & textColor, RGBColor const & backgroundColor) override;
	void textRect(Rect const & openrect, NativeString const & text, TextParams const & params = TextParams()) override;
	WDims measureText(NativeString const & text) override;

	// TODO make this part of the Canvas interface
	void drawImage(tk::image::Ptr const & image, tk::Point pos);

	// should be the last method to be called. calling any other methods, including this one after that is UB
	virtual tk::image::Ptr finishAndCreateImage() = 0;
private:
	virtual tk::Canvas & imageCanvas() = 0;
};

}

#endif
