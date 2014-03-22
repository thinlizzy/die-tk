#ifndef POINT_H_DIE_TK_2014_03_22_11_14
#define	POINT_H_DIE_TK_2014_03_22_11_14

#include <ostream>

namespace tk {

// TODO make a basic_point<> template
    
class Point {
public:
	int x,y;

	constexpr Point():
		x(),y()
	{}
	constexpr Point(int x_, int y_):
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

}

#endif
