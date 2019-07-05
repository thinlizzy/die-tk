#ifndef NULLIMAGE_H_DIE_TK_2015_04_01
#define NULLIMAGE_H_DIE_TK_2015_04_01

#include "Image.h"

namespace tk::image {

class NullImage: public Image {
public:
    unsigned bpp() const override { return 0; }
    WDims dims() const override { return WDims(); }
    Canvas & beginDraw() override { return canvas(); }
    Canvas & canvas() override;
    void endDraw() override {}
};

extern Ptr nullImage;

}

#endif
