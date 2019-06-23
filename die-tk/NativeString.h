#ifndef NATIVE_STRING_H_32139idfjksfdjfsd
#define NATIVE_STRING_H_32139idfjksfdjfsd

#include <string>
#include <ostream>

namespace tk {

class NativeString {
public:
    enum Encoding { utf8=0, utf16=1, utf32=2, };      // add more encodings according to the supported implementations

    union { 
        std::string str;
        std::wstring wstr;
    };
    
    NativeString();
    ~NativeString();
    NativeString(NativeString const & other);
	NativeString(NativeString && other);
    NativeString & operator=(NativeString const & other);
	NativeString & operator=(NativeString && other);
    NativeString(std::string const & strUTF8);
    NativeString(std::wstring const & strUTF16);
    NativeString(char ch);
    NativeString(char const * strUTF8);
    NativeString(char const * strUTF8, size_t size);
    NativeString(wchar_t const * strUTF16);
    NativeString(Encoding encoding, std::string const & strEncoded);
    
    std::string toUTF8() const;
    std::wstring toUTF16() const;
    
    int toInt() const;
    float toFloat() const;
    double toDouble() const;
    static NativeString toString(int value);
    static NativeString toString(float value);
    static NativeString toString(double value);
    
    bool empty() const;
    NativeString & operator+=(NativeString const & other);
    bool operator==(NativeString const & other) const;
    bool operator!=(NativeString const & other) const;
    
    static Encoding const nativeEncoding;
};

inline NativeString operator+(NativeString a, NativeString const & b) { return a+=b; }
bool operator<(NativeString const & a, NativeString const & b);

inline std::ostream & operator<<(std::ostream & os, NativeString const & str) { return os << str.toUTF8(); }
inline std::wostream & operator<<(std::wostream & os, NativeString const & str) { return os << str.toUTF16(); }

}

inline tk::NativeString operator"" _ns(char const * str, size_t size) { return tk::NativeString(str,size); }


#endif
