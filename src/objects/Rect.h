#ifndef RECT_H_DIE_TK_2014_03_22_11_18
#define	RECT_H_DIE_TK_2014_03_22_11_18

#include "Point.h"
#include "Dimension.h"
#include <ostream>
#include <algorithm>

namespace tk {

// TODO make a basic_rect<> template. also make it interact with basic_point<U>, instead

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

	static constexpr Rect open(Point p, WDims dims) { return Rect(p.x,p.y,p.x+dims.width,p.y+dims.height); }

	static constexpr Rect closed(Point p, WDims dims) { return open(p,dims+WDims(-1,-1)); }
    
	static constexpr Rect square(Point p, int size) { return open(p,WDims(size-1,size-1)); }

    constexpr int width() const { return right-left+1; }
    constexpr int height() const { return bottom-top+1; }
	constexpr WDims dims() const { return WDims(width(),height()); }

	constexpr Point topLeft() const { return Point(left,top); }
	constexpr Point topRight() const { return Point(right,top); }
	constexpr Point bottomLeft() const { return Point(left,bottom); }
	constexpr Point bottomRight() const { return Point(right,bottom); }
	constexpr Point center() const { return Point((right+left)/2,(bottom+top)/2); }
	[[deprecated]]
	constexpr Point pos() const { return topLeft(); }

    constexpr Rect addX(int x) const { return Rect(left+x,top,right+x,bottom); }
    constexpr Rect addY(int y) const { return Rect(left,top+y,right,bottom+y); }
    constexpr Rect add(Point p) const { return Rect(left+p.x,top+p.y,right+p.x,bottom+p.y); }
    
	constexpr Rect move(Point p) const { return Rect(p.x,p.y,p.x+right-left,p.y+bottom-top); }

	constexpr Rect resize(WDims dims) const { return closed(topLeft(),dims); }

	constexpr Rect resizeBottomRight(WDims dims) const { return Rect(right-dims.width+1,bottom-dims.height+1,right,bottom); }
    
    constexpr Point posDown(int margin) const { return topLeft().addY(height() + margin); }
    constexpr Point posRight(int margin) const { return topLeft().addX(width() + margin); }

    constexpr Point fitPoint(Point point) const
    {
    	return Point(
			std::max(this->left,std::min(this->right,point.x)),
			std::max(this->top,std::min(this->bottom,point.y)));
    }

    constexpr Rect fitInRect(Rect rect) const
    {
    	return Rect(
			this->left >= rect.left ? this->left : rect.left, this->top >= rect.top ? this->top : rect.top,
			this->right <= rect.right ? this-> right : rect.right, this->bottom <= rect.bottom ? this-> bottom : rect.bottom
    			);
    }

	constexpr bool contains(Rect const & rect) const
	{
		return top <= rect.top && left <= rect.left && bottom >= rect.bottom && right >= rect.right;
	}

	constexpr bool intersect(Point const & p) const
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

inline Intersection getIntersection(Rect const & rect1, Rect const & rect2)
{
    int l,r,t,b;
    l = std::max(rect1.left,rect2.left);
    r = std::min(rect1.right,rect2.right);
    if( l > r ) return Intersection();
    
    t = std::max(rect1.top,rect2.top);
    b = std::min(rect1.bottom,rect2.bottom);
    if( t > b ) return Intersection();
    
    return Intersection{ WDims{r-l+1,b-t+1}, Point{l,t} };
}

}

#endif
