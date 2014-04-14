#ifndef RECT_H_DIE_TK_2014_03_22_11_18
#define	RECT_H_DIE_TK_2014_03_22_11_18

#include "Point.h"
#include "Dimension.h"
#include <ostream>

namespace tk {

// TODO make a basic_rect<> template

class Rect {
public:
	int left,top;
	int right,bottom;

	constexpr Rect():
		left(), top(), right(), bottom()
	{}
	constexpr Rect(int left_, int top_, int right_, int bottom_):
		left(left_), top(top_), right(right_), bottom(bottom_)
	{}
	constexpr Rect(Point p1, Point p2):
		left(p1.x), top(p1.y), right(p2.x), bottom(p2.y)
	{}

	static Rect open(Point p, WDims dims) { return Rect(p.x,p.y,p.x+dims.width,p.y+dims.height); }

	static Rect closed(Point p, WDims dims) { return open(p,dims+WDims(-1,-1)); }
    
	static Rect square(Point p, int size) { return open(p,WDims(size-1,size-1)); }

    constexpr int width() const { return right-left+1; }
    constexpr int height() const { return bottom-top+1; }

	Point pos() const { return Point(left,top); }
	constexpr WDims dims() const { return WDims(width(),height()); }

    Rect addX(int x) const { return Rect(left+x,top,right+x,bottom); }
    Rect addY(int y) const { return Rect(left,top+y,right,bottom+y); }
    Rect addY(Point p) const { return Rect(left+p.x,top+p.y,right+p.x,bottom+p.y); }
    
	Rect move(Point p) const { return Rect(p.x,p.y,right+p.x-left,bottom+p.y-top); }

	Rect resize(WDims dims) const { return closed(pos(),dims); }
    
    Point posDown(int margin) const { return pos().addY(height() + margin); }
    Point posRight(int margin) const { return pos().addX(width() + margin); }

	bool intersect(Point const & p) const
	{
		return left <= p.x && p.x <= right &&
			top <= p.y && p.y <= bottom;
	}
    
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

	friend bool operator==(Rect const & r1, Rect const & r2)
	{
	    return
            r1.left == r2.left &&
            r1.right == r2.right &&
            r1.top == r2.top &&
            r1.bottom == r2.bottom;
	}
    
    friend bool operator!=(Rect const & r1, Rect const & r2)
    {
        return ! (r1 == r2);
    }

	friend std::ostream & operator<<(std::ostream & os, Rect const & rect)
	{
	    os << '(' << rect.left << ',' << rect.top << ")-(" << rect.right << ',' << rect.bottom << ')';
	    return os;
    }
};

// Intersection returns the result of two overlapping rect objects
// TODO verify if a Rect can be better instead of creating another class

struct Intersection {
    WDims dims;
    Point point;
    explicit operator bool() const { return ! dims.empty(); }
};

inline bool getIntersect(int & dim, int & p, int lim1, int lim2)
{
    dim = lim1 - lim2 + 1;
    if( dim <= 0 ) return false;

    p = lim1 - dim + 1;
    return true;    
}

inline bool getIntersectX(Intersection & result, Rect const & rect1, Rect const & rect2)
{
    return getIntersect(result.dims.width,result.point.x,rect1.right,rect2.left);
}

inline bool getIntersectY(Intersection & result, Rect const & rect1, Rect const & rect2)
{
    return getIntersect(result.dims.height,result.point.y,rect1.bottom,rect2.top);
}

inline Intersection getIntersection(Rect const & rect1, Rect const & rect2)
{
    Intersection result;
    
    if( rect1.left < rect2.left ) {
        if( ! getIntersectX(result,rect1,rect2) ) return Intersection();
    } else {
        if( ! getIntersectX(result,rect2,rect1) ) return Intersection();
    }
    
    if( rect1.top < rect2.top ) {
        if( ! getIntersectY(result,rect1,rect2) ) return Intersection();
    } else {
        if( ! getIntersectY(result,rect2,rect1) ) return Intersection();
    }
    
    return result;
}

}

#endif
