#ifndef DRAWING_H_DIE_TK_2014_03_22_11_29
#define	DRAWING_H_DIE_TK_2014_03_22_11_29

#include "Color.h"

namespace tk {

enum class PenStyle {
    solid, dash, dot, dashdot, dashdotdot, invisible,
};

struct Pen {
	RGBColor color;
	unsigned width;
	PenStyle style;
    
	Pen(RGBColor const & color = RGBColor()):
		color(color),width(),style()
	{}
    Pen & setWidth(unsigned width) { this->width = width; return *this; }
    Pen & setStyle(PenStyle style) { this->style = style; return *this; }
};

// TODO add styles for brushes

struct Brush {
	RGBColor color;
	constexpr Brush(RGBColor const & color = RGBColor()): color(color) {}
};

}

#endif
