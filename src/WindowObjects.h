#ifndef WINDOW_OBJECTS_H_fdsf4394kf2302k4bng
#define WINDOW_OBJECTS_H_fdsf4394kf2302k4bng

#include "Color.h"
#include "Dimension.h"
#include <ostream>

namespace tk {

enum WindowState { ws_destroyed=1, ws_visible=2, ws_minimized=4, ws_maximized=8, };

enum WindowKey {
	k_NONE, k_ESCAPE, k_SPACE, k_BACK, k_BACKSPACE=k_BACK, k_RETURN, k_ENTER=k_RETURN, k_TAB,
	k_INSERT, k_DELETE, k_HOME, k_END, k_PAGEUP, k_PAGEDOWN, k_PGUP=k_PAGEUP, k_PGDN=k_PAGEDOWN,
	k_UP, k_DOWN, k_LEFT, k_RIGHT, k_SHIFT, k_CONTROL,
	k_A, k_B, k_C, k_D, k_E, k_F, k_G, k_H, k_I, k_J, k_K, k_L, k_M,
	k_N, k_O, k_P, k_Q, k_R, k_S, k_T, k_U, k_V, k_W, k_X, k_Y, k_Z,
};

enum Cursor {
    cur_default = -1,
    cur_arrow, cur_wait, cur_smallwait, cur_hand, cur_edit, cur_help, cur_cross,
    cur_numCursors,
};

enum Scrollbar { sb_none, sb_horizontal, sb_vertical, sb_both, };

typedef basic_dimension<int> WDims;

typedef Color<unsigned char> RGBColor;

class Point {
public:
	int x,y;

	Point():
		x(),y()
	{}
	Point(int x_, int y_):
		x(x_), y(y_)
	{}
	Point setX(int x) const { return Point(x,y); }
	Point setY(int y) const { return Point(x,y); }
	Point addX(int x) const { return Point(this->x + x,y); }
	Point addY(int y) const { return Point(x,this->y + y); }

	bool operator==(Point const & p) const { return p.x == x && p.y == y; }
	Point & operator+=(Point const & p) { x += p.x;  y += p.y; return *this; }
	friend Point operator+(Point const & p1, Point const & p2) { return Point(p1)+=p2; }
	Point & operator-=(Point const & p) { x -= p.x;  y -= p.y; return *this; }
	friend Point operator-(Point const & p1, Point const & p2) { return Point(p1)-=p2; }

	friend std::ostream & operator<<(std::ostream & os, Point const & p) { os << '(' << p.x << ',' << p.y << ')'; return os; }
};

class Rect {
public:
	int left,top;
	int right,bottom;

	Rect():
		left(), top(), right(), bottom()
	{}
	Rect(int left_, int top_, int right_, int bottom_):
		left(left_), top(top_), right(right_), bottom(bottom_)
	{}
	Rect(Point p1, Point p2):
		left(p1.x), top(p1.y), right(p2.x), bottom(p2.y)
	{}

	static Rect open(Point p, WDims dims) { return Rect(p.x,p.y,p.x+dims.width,p.y+dims.height); }

	static Rect closed(Point p, WDims dims) { return open(p,dims+WDims(-1,-1)); }

	Point pos() const { return Point(left,top); }

	WDims openDims() const { return WDims(right-left,bottom-top); }

	WDims dims() const { return WDims(right-left+1,bottom-top+1); }

	Rect move(Point p) const { return Rect(p.x,p.y,right+p.x-left,bottom+p.y-top); }

	Rect resize(WDims dims) const { return closed(pos(),dims); }

	bool intersect(Rect const & rect) const
	{
		if( left <= rect.left ) {
			if( right >= rect.left ) {
				if( top <= rect.top ) {
					return bottom >= rect.top;
				} else {
					return top <= rect.bottom;
				}
			}
		} else {
			if( left <= rect.right ) {
				if( top <= rect.top ) {
					return bottom >= rect.top;
				} else {
					return top <= rect.bottom;
				}
			}
		}
		return false;
	}

	bool intersect(Point const & p) const
	{
		return left <= p.x && p.x <= right &&
			top <= p.y && p.y <= bottom;
	}

	friend bool operator==(Rect const & r1, Rect const & r2)
	{
	    return
            r1.left == r2.left &&
            r1.right == r2.right &&
            r1.top == r2.top &&
            r1.bottom == r2.bottom;
	}

	friend std::ostream & operator<<(std::ostream & os, Rect const & rect)
	{
	    os << '(' << rect.left << ',' << rect.top << ")-(" << rect.right << ',' << rect.bottom << ')';
	    return os;
    }
};

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

enum ImageType { it_native, it_RGB, it_BGR, it_gray, };

struct ImageRef {
    ImageType type;
    union Metadata {
        void * nativeHeader;
        WDims dimensions;
        Metadata() {}
    } metadata;
    unsigned char const * buffer;

    static ImageRef native(void * nativeHeader, unsigned char const * buffer) {
        ImageRef result;
        result.type = it_native;
        result.buffer = buffer;
        result.metadata.nativeHeader = nativeHeader;
        return result;
    }

    static ImageRef raw(ImageType type, WDims dimensions, unsigned char const * buffer) {
        ImageRef result;
        result.type = type;
        result.buffer = buffer;
        result.metadata.dimensions = dimensions;
        return result;
    }
};

}

#endif
