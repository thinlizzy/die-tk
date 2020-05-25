#ifndef DIMENSION_H_A81BA61C_1651_4c9f_8EEF_4365745FDEFB
#define DIMENSION_H_A81BA61C_1651_4c9f_8EEF_4365745FDEFB

#include <ostream>
#include <algorithm>

namespace tk {

/** this class template represents a two-dimension bounds */
template<typename T>
struct basic_dimension {
	typedef T DimType;
	DimType width;		/** width */
	DimType height;		/** height */

	constexpr static basic_dimension squared(T size) { return basic_dimension(size,size); }

	constexpr basic_dimension():
        width(0),
        height(0)
    {}

	constexpr basic_dimension(T width, T height):
        width(width),
        height(height)
    {}

	template<typename U>
	explicit constexpr basic_dimension(basic_dimension<U> const & d):
        width(d.width),
        height(d.height)
    {}

	template<typename U>
	basic_dimension & assign(basic_dimension<U> const & d);

	bool empty() const;

	bool isRectangle() const;

	std::size_t area() const;

	constexpr basic_dimension setWidth(DimType width) const
    {
        return basic_dimension<T>(width, this->height);
    }
	constexpr basic_dimension setHeight(DimType height) const
    {
        return basic_dimension<T>(this->width, height);
    }
	constexpr basic_dimension addWidth(DimType width) const
    {
        return basic_dimension<T>(this->width + width, this->height);
    }
	constexpr basic_dimension addHeight(DimType height) const
    {
        return basic_dimension<T>(this->width, this->height + height);
    }

	constexpr basic_dimension fitInto(basic_dimension dims) const
    {
        return basic_dimension<T>(std::min(this->width,dims.width), std::min(this->height,dims.height));
    }

	basic_dimension & operator+=(basic_dimension const & d);

	basic_dimension & operator-=(basic_dimension const & d);

	basic_dimension & operator*=(basic_dimension const & d);

	basic_dimension & operator/=(basic_dimension const & d);

	template<typename U>
	basic_dimension & operator*=(U f);

	template<typename U>
	basic_dimension & operator/=(U f);
    
	friend std::ostream & operator<<(std::ostream & os, basic_dimension const & dims) { os << '(' << dims.width << ',' << dims.height << ')'; return os; }
};


template<typename T>
bool operator==(basic_dimension<T> const & d1, basic_dimension<T> const & d2);

template<typename T>
bool operator!=(basic_dimension<T> const & d1, basic_dimension<T> const & d2);

template<typename T>
constexpr basic_dimension<T> operator+(basic_dimension<T> const & d1, basic_dimension<T> const & d2) {
    return basic_dimension<T>(d1.width+d2.width,d1.height+d2.height);
}

template<typename T>
constexpr basic_dimension<T> operator-(basic_dimension<T> const & d1, basic_dimension<T> const & d2) {
    return basic_dimension<T>(d1.width-d2.width,d1.height-d2.height);
}

template<typename T, typename U>
constexpr basic_dimension<T> operator*(basic_dimension<T> const & d1, U f);

template<typename T, typename U>
constexpr basic_dimension<T> operator*(U f, basic_dimension<T> const & d1);

template<typename T, typename U>
basic_dimension<T> operator/(basic_dimension<T> const & d1, U f);

template<typename T>
basic_dimension<T> operator*(basic_dimension<T> const & d1, basic_dimension<T> const & d2);

template<typename T>
basic_dimension<T> operator/(basic_dimension<T> const & d1, basic_dimension<T> const & d2);

/** common dimension type */
typedef basic_dimension<int> Dimension;

inline Dimension rDimension(unsigned h, unsigned w) { return Dimension(w,h); }

#include "Dimension_impl.h"

typedef basic_dimension<int> WDims;

namespace dims_literals {

constexpr WDims operator""_w(unsigned long long w)
{
	return WDims{}.setWidth(w);
}

constexpr WDims operator""_h(unsigned long long h)
{
	return WDims{}.setHeight(h);
}

}

}

#endif
