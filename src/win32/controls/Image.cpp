#include "../../controls/Image.h"
#include "ImageWin32.h"

#define CONTROL_IMPL ImageImpl
#include "ControlMacros.h"

namespace tk {

CONSTRUCTOR_IMPL(Image);
    
void Image::setImage(unsigned char const buffer[], void * header)
{
    IMPL.setImage(buffer,header);
}

void Image::setAutosize(bool autosize)
{
    IMPL.setAutosize(autosize);    
}

}
