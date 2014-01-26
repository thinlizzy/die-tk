#ifndef NULLCANVAS_H_STARSHIP_PC_2014_01_26_03_47
#define	NULLCANVAS_H_STARSHIP_PC_2014_01_26_03_47

#include "Canvas.h"

namespace tk {

class NullCanvas: public Canvas {
	virtual void setPen(Pen const & pen) {}
	virtual void setBrush(Brush const & brush) {}

	virtual void translate(Point p) {}
	virtual void clearTranslate() {}
	virtual void addClipRect(Rect const & openrect) {}
	virtual void clearClipping() {}

	virtual void plot(Point p, RGBColor const & color) {}
	virtual void drawLine(Point p1, Point p2) {}
	virtual void drawPoly(Points const & polygon) {}

	virtual void rectangle(Rect const & rect) {}
	virtual void fillRect(Rect const & openrect) {}

	virtual void textRect(Rect const & openrect, die::NativeString const & text, TextParams const & params = TextParams()) {}
    virtual WDims measureText(die::NativeString const & text) { return WDims(); }
    
    virtual void draw(image::Ptr image, Point dest) {}
    virtual void draw(image::Ptr image, Rect destrect) {}
};

extern NullCanvas nullCanvas;

}

#endif