#include "../src/convert.h"
#include <die-tk-win32.h>

namespace tk {

image::Ptr convertImage(img::Image const & image)
{
	auto info = reinterpret_cast<BITMAPINFO *>(image.getWindowSystemHeader());
	if( image.transparent() ) {
        if( info->bmiHeader.biBitCount == 32 ) {
            return std::make_shared<image::BitmapAlpha>(info,image.rawBits());
        } else {
            return std::make_shared<image::BitmapPallete>(info,image.rawBits(),image.getTransparentColorIndex());
        }
	} else {
        return std::make_shared<image::Bitmap>(info,image.rawBits());
	}
}

}
