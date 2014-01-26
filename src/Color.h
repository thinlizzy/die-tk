#ifndef COLOR_H_3B31924C_0E44_411c_8E83_5EB755219EAD
#define COLOR_H_3B31924C_0E44_411c_8E83_5EB755219EAD

#include <ostream>

// Color //

namespace tk {

template<typename T>
struct Color_traits { typedef T intensity_type; };

template<>
struct Color_traits<char> { typedef long intensity_type; };

template<>
struct Color_traits<unsigned char> { typedef unsigned long intensity_type; };

template<typename T>
struct Color {
	T r,g,b;
	constexpr Color(): r(),g(),b() {}
	constexpr Color(T r, T g, T b): r(r),g(g),b(b) {}
	template<typename U>
	constexpr Color(Color<U> const & c): r(c.r), g(c.g), b(c.b) {}
	Color & operator+=(Color const & c) { r+=c.r; g+=c.g; b+=c.b; return *this; }
	Color & operator/=(T div) { r/=div; g/=div; b/=div; return *this; }
	friend bool operator==(Color const & c1, Color const & c2) { return c1.r == c2.r && c1.g == c2.g && c1.b == c2.b; }
	friend bool operator!=(Color const & c1, Color const & c2) { return ! (c1 == c2); }
	friend Color operator+(Color const & c1, Color const & c2) { return Color(c1)+=c2; }
	friend Color operator/(Color const & c, T div) { return Color(c)/=div; }
	constexpr typename Color_traits<T>::intensity_type rgbsum() const { return r+g+b; }
    
	friend std::ostream & operator<<(std::ostream & os, Color const & c) { 
        os << '(' << std::hex << int(c.r) << ',' << int(c.g) << ',' << int(c.b) << std::dec << ')'; 
        return os; 
    }
};

}

#endif
