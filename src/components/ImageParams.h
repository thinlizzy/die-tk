#ifndef IMAGEPARAMS_H_DIE_TK_2014_04_01
#define IMAGEPARAMS_H_DIE_TK_2014_04_01

#include "../objects/Dimension.h"

namespace tk {
namespace image {

enum class Type { RGBA, BGRA, RGB, BGR, gray, }; // TODO add more types

typedef unsigned char Byte;

struct Params {
    Byte * buffer_;
    // TODO remove native header
    void * nativeHeader_;
    Type type_;
    WDims dimensions_;
    int transparentIndex_ = -1;
    bool externalBuffer_ = false;
    bool tryTransparent_ = true;

    // TODO remove this constructor
    Params(void * nativeHeader, Byte * buffer = 0):
        buffer_(buffer),
        nativeHeader_(nativeHeader)
    {}

    Params(Type type, WDims dimensions, Byte * buffer = 0):
        buffer_(buffer),
        nativeHeader_(0),
        type_(type),
        dimensions_(dimensions)
    {}

    Params & buffer(Byte * value) { buffer_ = value; return *this; }
    Params & type(Type value) { type_ = value; return *this; }
    Params & nativeHeader(void * value) { nativeHeader_ = value; return *this; }
    Params & dimensions(WDims value) { dimensions_ = value; return *this; }

    Params & transparentIndex(int value) { transparentIndex_ = value; return *this; }
    Params & externalBuffer(bool value = true) { externalBuffer_ = value; return *this; }
    Params & forceNoTransparent(bool value = true) { tryTransparent_ = ! value; return *this; }

    unsigned bpp() const {
    	switch(type_) {
    	case Type::RGBA:
    	case Type::BGRA:
    		return 32;
    	case Type::RGB:
    	case Type::BGR:
    		return 24;
    	case Type::gray:
    		return 8;
    	}
    	return 0;
    }

    size_t totalBytes() const {
    	return dimensions_.area() * (bpp() / 8);
    }
};

}
}

#endif
