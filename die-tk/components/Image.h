#ifndef IMAGE_H_DIE_TK_2014_01_18_14_20
#define IMAGE_H_DIE_TK_2014_01_18_14_20

#include "../objects/Dimension.h"
#include "../objects/Point.h"
#include "../objects/Rect.h"
#include "ImageParams.h"
#include <memory>

namespace tk {

class Canvas;

namespace image {

class Image {
public:
    virtual ~Image() {};
    virtual unsigned bpp() const = 0;
    virtual WDims dims() const = 0;
    virtual Canvas & beginDraw() = 0;
    virtual Canvas & canvas() = 0;
    virtual void endDraw() = 0;
    virtual void drawInto(Canvas & canvas, Point dest) = 0;
    virtual void drawInto(Canvas & canvas, Rect destrect) = 0;
    virtual void copyRectInto(Canvas & canvas, Rect srcrect, Point dest) = 0;
};

typedef std::shared_ptr<Image> Ptr;

Ptr create(Params const & params);

}

}

#endif
