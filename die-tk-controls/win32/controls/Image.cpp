#include "../../src/controls/Image.h"
#include "ImageWin32.h"

#define CONTROL_IMPL ImageImpl
#include "ControlMacros.h"

namespace tk {

CONSTRUCTOR_IMPL(Image);
CLONE_IMPL(Image);
    
void Image::setImage(image::Ptr image)
{
    IMPL.setImage(image);
}

void Image::setAutosize(bool autosize)
{
    IMPL.setAutosize(autosize);    
}

Canvas & Image::beginDraw()
{
    return IMPL.beginDraw();
}

void Image::endDraw()
{
    IMPL.endDraw();
}

}
