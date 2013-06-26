#ifndef CANVAS_H_6CD1490B_F5E5_4985_8494_ADEE0B297978
#define CANVAS_H_6CD1490B_F5E5_4985_8494_ADEE0B297978

#include "WindowObjects.h"
#include <vector>
#include <NativeString.h>

namespace tk {

typedef std::vector<Point> Points;

class Canvas {
public:
	virtual ~Canvas() {}
	virtual void setPen(Pen const & pen) = 0;
	virtual void setBrush(Brush const & brush) = 0;

	virtual void translate(Point p) = 0;
	virtual void clearTranslate() = 0;
	virtual void addClipRect(Rect const & openrect) = 0;
	virtual void clearClipping() = 0;

	virtual void plot(Point p, RGBColor const & color) = 0;
	virtual void drawLine(Point p1, Point p2) = 0;
	virtual void drawPoly(Points const & polygon) = 0;

	virtual void rectangle(Rect const & rect) = 0;
	virtual void fillRect(Rect const & openrect) = 0;

	virtual void drawImage(ImageRef ih, Point start = Point()) = 0;
	virtual void drawImage(ImageRef ih, Rect destrect) = 0;

	virtual void textRect(Rect const & openrect, die::NativeString const & text, TextParams const & params = TextParams()) = 0;
};

}


#endif
