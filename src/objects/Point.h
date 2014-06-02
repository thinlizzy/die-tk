#ifndef POINT_H_DIE_TK_2014_03_22_11_14
#define	POINT_H_DIE_TK_2014_03_22_11_14

#include <ostream>
#include <cmath>

namespace tk {

template<typename T>    
class basic_point {
public:
    typedef T value_type;
	T x,y;

	constexpr basic_point():
		x(),y()
	{}
	constexpr basic_point(T x, T y):
		x(x), y(y)
	{}
	template<typename U>
	constexpr explicit basic_point(basic_point<U> const & p):
		x(p.x), y(p.y)
    {}
	basic_point setX(T x) const { return basic_point(x,this->y); }
	basic_point setY(T y) const { return basic_point(this->x,y); }
	basic_point addX(T x) const { return basic_point(this->x + x,y); }
	basic_point addY(T y) const { return basic_point(x,this->y + y); }

	bool operator==(basic_point const & p) const { return p.x == x && p.y == y; }
    bool operator!=(basic_point const & p) const { return ! operator==(p); }
	basic_point & operator+=(basic_point const & p) { x += p.x;  y += p.y; return *this; }
	friend basic_point operator+(basic_point const & p1, basic_point const & p2) { return basic_point(p1)+=p2; }
	basic_point & operator-=(basic_point const & p) { x -= p.x;  y -= p.y; return *this; }
	friend basic_point operator-(basic_point const & p1, basic_point const & p2) { return basic_point(p1)-=p2; }
	basic_point & operator*=(T f) { x *= f;  y *= f; return *this; }
	friend basic_point operator*(basic_point const & p, T f) { return basic_point(p)*=f; }
	friend basic_point operator*(T f, basic_point const & p) { return basic_point(p)*=f; }
	basic_point & operator/=(T f) { x /= f;  y /= f; return *this; }
	friend basic_point operator/(basic_point const & p, T f) { return basic_point(p)/=f; }
    
    constexpr T distance() const { return std::sqrt(x*x+y*y); }

	friend std::ostream & operator<<(std::ostream & os, basic_point const & p) { os << '(' << p.x << ',' << p.y << ')'; return os; }
};

typedef basic_point<int> Point;

}

#endif
