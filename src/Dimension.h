#ifndef DIMENSION_H_A81BA61C_1651_4c9f_8EEF_4365745FDEFB
#define DIMENSION_H_A81BA61C_1651_4c9f_8EEF_4365745FDEFB

#include <ostream>

namespace tk {

/** this class template represents a two-dimension bounds */
template<typename T>
struct basic_dimension {
	typedef T DimType;
	DimType width;		/** width */
	DimType height;		/** height */

	constexpr basic_dimension():
        width(0),
        height(0)
    {}

	constexpr basic_dimension(T width, T height):
        width(width),
        height(height)
    {}

	template<typename U>
	constexpr basic_dimension(basic_dimension<U> const & d):
        width(d.width),
        height(d.height)
    {}

	template<typename U>
	void assign(basic_dimension<U> const & d);

	bool empty() const;

	bool isRectangle() const;

	std::size_t area() const;

	basic_dimension setWidth(DimType width) const;
	basic_dimension setHeight(DimType height) const;
	basic_dimension addWidth(DimType width) const;
	basic_dimension addHeight(DimType height) const;

	basic_dimension & operator+=(basic_dimension const & d);

	basic_dimension & operator-=(basic_dimension const & d);

	basic_dimension & operator*=(basic_dimension const & d);

	basic_dimension & operator/=(basic_dimension const & d);

	basic_dimension & operator*=(T f);

	basic_dimension & operator/=(T f);
    
	friend std::ostream & operator<<(std::ostream & os, basic_dimension const & dims) { os << '(' << dims.width << ',' << dims.height << ')'; return os; }
};


template<typename T>
bool operator==(basic_dimension<T> const & d1, basic_dimension<T> const & d2);

template<typename T>
bool operator!=(basic_dimension<T> const & d1, basic_dimension<T> const & d2);

template<typename T>
basic_dimension<T> const operator+(basic_dimension<T> const & d1, basic_dimension<T> const & d2);

template<typename T>
basic_dimension<T> const operator-(basic_dimension<T> const & d1, basic_dimension<T> const & d2);

template<typename T>
basic_dimension<T> const operator*(basic_dimension<T> const & d1, basic_dimension<T> const & d2);

template<typename T>
basic_dimension<T> const operator/(basic_dimension<T> const & d1, basic_dimension<T> const & d2);

template<typename T>
basic_dimension<T> const operator*(basic_dimension<T> const & d1, typename basic_dimension<T>::DimType f);

template<typename T>
basic_dimension<T> const operator*(typename basic_dimension<T>::DimType f, basic_dimension<T> const & d1);

template<typename T, typename U>
basic_dimension<U> const operator*(basic_dimension<T> const & d1, typename basic_dimension<U>::DimType f);

template<typename T, typename U>
basic_dimension<U> const operator*(typename basic_dimension<U>::DimType f, basic_dimension<T> const & d1);

template<typename T>
basic_dimension<T> const operator/(basic_dimension<T> const & d1, typename basic_dimension<T>::DimType f);

template<typename T, typename U>
basic_dimension<U> const operator/(basic_dimension<T> const & d1, typename basic_dimension<U>::DimType f);

/** common dimension type */
typedef basic_dimension<int> Dimension;

inline Dimension rDimension(unsigned h, unsigned w) { return Dimension(w,h); }

#include "Dimension_impl.h"

}

#endif
