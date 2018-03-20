#ifndef NULLIMAGE_H_DIE_TK_2015_04_01
#define NULLIMAGE_H_DIE_TK_2015_04_01

#include "Image.h"

namespace tk {
namespace image {

class NullImage: public Image {
public:
    unsigned bpp() const override { return 0; }
    WDims dims() const override { return WDims(); }
    Canvas & beginDraw() override { return canvas(); }
    Canvas & canvas() override;
    void endDraw() override {}
    void drawInto(Canvas & canvas, Point dest) override {}
    void drawInto(Canvas & canvas, Rect destrect) override {}
    void copyRectInto(Canvas & canvas, Rect srcrect, Point dest) {}
};

extern Ptr nullImage;

}
}

#endif
