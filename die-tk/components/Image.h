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
    virtual ~Image() = default;
    virtual unsigned bpp() const = 0;
    virtual WDims dims() const = 0;
    virtual Canvas & beginDraw() = 0;
    virtual Canvas & canvas() = 0;
    virtual void endDraw() = 0;
    // TODO remove drawInto() from the Image interface in favor of ImageCanvas
    virtual void drawInto(Canvas & canvas, Point dest) = 0;
	// TODO move drawInto() from the Image interface to Canvas
    virtual void drawInto(Canvas & canvas, Rect destrect) = 0;
	// TODO move copyRectInto() from the Image interface to Canvas
    virtual void copyRectInto(Canvas & canvas, Rect srcrect, Point dest) = 0;
};

using Ptr = std::shared_ptr<Image>;

Ptr create(Params const & params);

}

}

#endif
