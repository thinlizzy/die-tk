#ifndef CANVASX11_H_DIE_TK_2014_OCT_12
#define CANVASX11_H_DIE_TK_2014_OCT_12

#include "../Canvas.h"
#include <X11/Xlib.h>

namespace tk {

/**
 * This class needs a lot of working and tailoring.
 * The clipping functions don't do what is expected and can generate undesirable side effects.
 * The text functions need improvement.
 */
class CanvasX11: public Canvas {
public:
	GC gc;
	Drawable drawable;

	CanvasX11();
	explicit CanvasX11(Drawable drawable);
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

	void drawText(Point p, NativeString const & text, RGBColor const & color) override;
	void textRect(Rect const & openrect, NativeString const & text, TextParams const & params = TextParams()) override;
    WDims measureText(NativeString const & text) override;

    void setForegroundColor(RGBColor const & color);
private:
    void setPen(Pen const & pen);
    void setBrush(Brush const & brush);
};

}

#endif
