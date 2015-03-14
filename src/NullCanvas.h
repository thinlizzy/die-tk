#ifndef NULLCANVAS_H_STARSHIP_PC_2014_01_26_03_47
#define	NULLCANVAS_H_STARSHIP_PC_2014_01_26_03_47

#include "Canvas.h"

namespace tk {

class NullCanvas: public Canvas {
	void translate(Point p) override {}
	void clearTranslate() override {}
	void addClipRect(Rect const & openrect) override {}
	void clearClipping() override {}

	void plot(Point p, RGBColor const & color) override {}
	void drawLine(Point p1, Point p2, Pen const & pen) override {}
	void drawPoly(Points const & polygon, Pen const & pen) override {}

	void rectangle(Rect const & rect, Pen const & pen) override {}
	void fillRect(Rect const & openrect, Brush const & brush) override {}

	void drawText(Point p, die::NativeString const & text, RGBColor const & color) override {}
	void textRect(Rect const & openrect, die::NativeString const & text, TextParams const & params = TextParams()) override {}
    WDims measureText(die::NativeString const & text) override { return WDims(); }
};

extern NullCanvas nullCanvas;

}

#endif
