#ifndef CANVAS_H_6CD1490B_F5E5_4985_8494_ADEE0B297978
#define CANVAS_H_6CD1490B_F5E5_4985_8494_ADEE0B297978

#include "WindowObjects.h"
#include <vector>
#include <string>

namespace tk {

enum HTextAlign { hta_left, hta_right, hta_center };
enum VTextAlign { vta_top, vta_bottom, vta_center };

class TextParams {
public:
	HTextAlign h_align;
	VTextAlign v_align;
	RGBColor textColor,backgroundColor;

	TextParams():
		h_align(hta_left),
		v_align(vta_center),
		textColor(0,0,0),
		backgroundColor(255,255,255)
	{}
	TextParams & horizontalAlign(HTextAlign align) { h_align = align; return *this; }
	TextParams & verticalAlign(VTextAlign align) { v_align = align; return *this; }
	TextParams & color(RGBColor const & color) { textColor = color; return *this; }
	TextParams & background(RGBColor const & color) { backgroundColor = color; return *this; }
};

struct Pen {
	RGBColor color;
	unsigned width;
	int style;
	Pen(): 
		color(),width(),style()
	{}
	Pen(RGBColor const & color):
		color(color),width(),style()
	{}
};

struct Brush {
	RGBColor color;
	Brush(): color() {}
	Brush(RGBColor const & color): color(color) {}
};

typedef std::vector<Point> Points;

enum ImageType { it_RGB, it_BGR, it_gray, };

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

	virtual void drawImage(Point start, WDims dim, unsigned char const buffer[], ImageType type) = 0;
	virtual void drawImage(Rect const & openrect, unsigned char const buffer[], ImageType type) = 0;
	virtual void drawImage(Point dest, unsigned char const buffer[], void * header) = 0;
	virtual void drawImage(Rect const & destrect, Point start, unsigned char const buffer[], void * header) = 0;

	virtual void textRect(Rect const & openrect, std::string const & text, TextParams const & params = TextParams()) = 0;
};

}


#endif
