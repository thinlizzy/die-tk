#ifndef IMAGE_H_DIE_TK_2014_01_18_14_20
#define IMAGE_H_DIE_TK_2014_01_18_14_20

#include "../objects/Dimension.h"
#include <memory>

namespace tk {

class Canvas;

namespace image {

enum class Type { RGB, BGR, gray, }; // TODO add more types

typedef unsigned char Byte;

struct Params {
    Byte * buffer_;
    void * nativeHeader_;
    Type type_;
    WDims dimensions_;
    int transparentIndex_ = -1;
    bool externalBuffer_ = false;
    bool tryTransparent_ = true;
    
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
};

class Image {
public:
    virtual ~Image() {};
    virtual unsigned bpp() const = 0;
    virtual WDims dims() const = 0;
    virtual Canvas & beginDraw() = 0;
    virtual Canvas & canvas() = 0;
    virtual void endDraw() = 0;
};

typedef std::shared_ptr<Image> Ptr;

extern Ptr nullImage;

Ptr create(Params const & params);

}

}

#endif
