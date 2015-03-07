#ifndef CANVASX11_H_DIE_TK_2014_OCT_12
#define CANVASX11_H_DIE_TK_2014_OCT_12

#include "../Canvas.h"
#include <X11/Xlib.h>

namespace tk {

class CanvasX11: public Canvas {
public:
	GC gc;
	::Window windowId;

	CanvasX11();
	explicit CanvasX11(::Window windowId);
	CanvasX11(CanvasX11 && other);
	CanvasX11 & operator=(CanvasX11 && other);
	~CanvasX11() override;

	void translate(Point p) override;
	void clearTranslate() override;
	void addClipRect(Rect const & openrect) override;
	void clearClipping() override;

	void plot(Point p, RGBColor const & color) override;
	void drawLine(Point p1, Point p2, Pen const & pen) override;
	void drawPoly(Points const & polygon, Pen const & pen) override;

	void rectangle(Rect const & rect, Pen const & pen) override;
	void fillRect(Rect const & openrect, Brush const & brush) override;

	void drawText(Point p, die::NativeString const & text, RGBColor const & color) override;
	void textRect(Rect const & openrect, die::NativeString const & text, TextParams const & params = TextParams()) override;
    WDims measureText(die::NativeString const & text) override;

    void draw(image::Ptr image, Point dest) override;
    void draw(image::Ptr image, Rect destrect) override;
private:
    void setForegroundColor(RGBColor const & color);
    void setPen(Pen const & pen);
    void setBrush(Brush const & brush);
};

}

#endif
