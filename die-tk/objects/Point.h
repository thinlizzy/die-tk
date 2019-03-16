#ifndef POINT_H_DIE_TK_2014_03_22_11_14
#define	POINT_H_DIE_TK_2014_03_22_11_14

#include <ostream>
#include <cmath>

namespace tk {

template<typename T>    
class basic_point {
public:
	using value_type = T;
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
	constexpr basic_point setX(T x) const { return basic_point(x,this->y); }
	constexpr basic_point setY(T y) const { return basic_point(this->x,y); }
	constexpr basic_point addX(T x) const { return basic_point(this->x + x,y); }
	constexpr basic_point addY(T y) const { return basic_point(x,this->y + y); }
    
    constexpr basic_point operator-() const { return basic_point(-x,-y); }

    constexpr bool operator==(basic_point const & p) const { return p.x == x && p.y == y; }
    constexpr bool operator!=(basic_point const & p) const { return ! operator==(p); }
	constexpr basic_point & operator+=(basic_point const & p) { x += p.x;  y += p.y; return *this; }
	friend constexpr basic_point operator+(basic_point const & p1, basic_point const & p2) { return basic_point(p1)+=p2; }
	constexpr basic_point & operator-=(basic_point const & p) { x -= p.x;  y -= p.y; return *this; }
	friend constexpr basic_point operator-(basic_point const & p1, basic_point const & p2) { return basic_point(p1)-=p2; }
	template<typename U>
	constexpr basic_point & operator*=(U f) { x *= f;  y *= f; return *this; }
	template<typename U>
	friend constexpr basic_point operator*(basic_point const & p, U f) { return basic_point(p)*=f; }
	template<typename U>
	friend constexpr basic_point operator*(U f, basic_point const & p) { return basic_point(p)*=f; }
	constexpr basic_point & operator/=(T f) { x /= f;  y /= f; return *this; }
	friend constexpr basic_point operator/(basic_point const & p, T f) { return basic_point(p)/=f; }

	template<typename U>
	constexpr basic_point scalar(U sx, U sy) const { return basic_point(x*sx,y*sy); }
	template<typename U>
	constexpr basic_point scalar(basic_point<U> const & p) const { return scalar(p.x,p.y); }

    constexpr T distance() const { return std::sqrt(x*x+y*y); }

	friend std::ostream & operator<<(std::ostream & os, basic_point const & p) { os << '(' << p.x << ',' << p.y << ')'; return os; }
};

using Point = basic_point<int>;

namespace point_literals {

constexpr Point operator""_x(unsigned long long v)
{
    return Point{}.setX(v);
}

constexpr Point operator""_y(unsigned long long v)
{
    return Point{}.setY(v);
}

}

}

#endif
