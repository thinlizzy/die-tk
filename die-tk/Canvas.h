#ifndef CANVAS_H_6CD1490B_F5E5_4985_8494_ADEE0B297978
#define CANVAS_H_6CD1490B_F5E5_4985_8494_ADEE0B297978

#include "NativeString.h"
#include "TextParams.h"
#include "components/Image.h"
#include "objects/Color.h"
#include "objects/Dimension.h"
#include "objects/Point.h"
#include "objects/Rect.h"
#include "objects/Drawing.h"
#include <vector>

namespace tk {

typedef std::vector<Point> Points;

class Canvas {
public:
	virtual ~Canvas() = default;

	virtual void translate(Point p) = 0;
	virtual void clearTranslate() = 0;
	virtual void addClipRect(Rect const & openrect) = 0;
	virtual void clearClipping() = 0;

	virtual void plot(Point p, RGBColor const & color) = 0;
	virtual void drawLine(Point p1, Point p2, Pen const & pen) = 0;
	virtual void drawPoly(Points const & polygon, Pen const & pen) = 0;

	virtual void rectangle(Rect const & rect, Pen const & pen) = 0;
	virtual void fillRect(Rect const & openrect, Brush const & brush) = 0;
	virtual void roundRect(Rect const & openrect, WDims ellipseDims, Pen const & pen, Brush const & brush) = 0;

	virtual void drawText(Point p, NativeString const & text, RGBColor const & color) = 0;
	virtual void drawText(Point p, NativeString const & text, RGBColor const & textColor, RGBColor const & backgroundColor) = 0;
	virtual void textRect(Rect const & openrect, NativeString const & text, TextParams const & params = TextParams()) = 0;
    virtual WDims measureText(NativeString const & text) = 0;

    virtual void drawImage(tk::image::Ptr const & image, tk::Point pos) = 0;
};

}


#endif
