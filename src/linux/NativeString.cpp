#include "../NativeString.h"
#include <iostream>
#include <sstream>
#include <cwchar>

#include <iterator>
#include <cstdint>
#include "utf8.h"

namespace die {

NativeString::Encoding const NativeString::nativeEncoding = NativeString::utf8;

template<typename IT>
std::wstring utf8toUtf16(IT beg, IT end)
{
	std::wstring result;
	utf8::utf8to16(beg,end,std::back_inserter(result));
	return result;
}

template<typename IT>
std::string utf16toUtf8(IT beg, IT end)
{
	std::string result;
	utf8::utf16to8(beg,end,std::back_inserter(result));
	return result;
}

std::string encodeToUTF8(NativeString::Encoding encoding, std::string const & strEncoded)
{
	if( strEncoded.empty() ) return std::string();

    switch(encoding) {
        case NativeString::utf16:
        	return utf16toUtf8(
        			reinterpret_cast<std::uint16_t const *>(&strEncoded[0]),
        			reinterpret_cast<std::uint16_t const *>(&strEncoded[strEncoded.size()]));
        case NativeString::utf8:
            return strEncoded;
        default:
            std::cerr << "unsupported encoding" << std::endl;
            return std::string();
    }
}

NativeString::NativeString():
    str()
{}

NativeString::~NativeString()
{
    using std::string;
    str.~string();
}

NativeString::NativeString(NativeString const & other):
    str(other.str)
{}

NativeString & NativeString::operator=(NativeString const & other)
{
    str = other.str;
    return *this;
}

NativeString::NativeString(std::string const & strUTF8):
    str(strUTF8)
{}

NativeString::NativeString(char const * strUTF8):
    str(strUTF8)
{}

NativeString::NativeString(char const * strUTF8, size_t size):
    str(strUTF8,size)
{}

NativeString::NativeString(NativeString::Encoding encoding, std::string const & strEncoded):
    str(encodeToUTF8(encoding,strEncoded))
{}

std::string NativeString::toUTF8() const
{
	return str;
}

NativeString::NativeString(std::wstring const & strUTF16):
    str(utf16toUtf8(strUTF16.begin(),strUTF16.end()))
{}

NativeString::NativeString(wchar_t const * strUTF16):
    str(utf16toUtf8(strUTF16,strUTF16 + std::wcslen(strUTF16)))
{}

std::wstring NativeString::toUTF16() const
{
    return utf8toUtf16(str.begin(),str.end());
}

template<typename T>
T lexical_cast(std::string const & str)
{
    T result = T();
    std::basic_istringstream<char> iss(str);
    iss >> result;
    return result;
}

int NativeString::toInt() const
{
    return lexical_cast<int>(str);
}

float NativeString::toFloat() const
{
    return lexical_cast<float>(str);
}

double NativeString::toDouble() const
{
    return lexical_cast<double>(str);
}

template<typename T>
NativeString toStringT(T value)
{
    std::basic_ostringstream<char> oss;
    oss << value;
    return NativeString(oss.str());
}

NativeString NativeString::toString(int value)
{
    return toStringT(value);
}

NativeString NativeString::toString(float value)
{
    return toStringT(value);
}

NativeString NativeString::toString(double value)
{
    return toStringT(value);
}

bool NativeString::empty() const
{
    return str.empty();
}

NativeString & NativeString::operator+=(NativeString const & other)
{
    str += other.str;
    return *this;
}

bool NativeString::operator==(NativeString const & other) const
{
    return str == other.str;
}

bool NativeString::operator!=(NativeString const & other) const
{
    return str != other.str;
}

bool operator<(NativeString const & a, NativeString const & b)
{
    return a.str < b.str;
}

}

