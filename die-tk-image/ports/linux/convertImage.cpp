#include "../../convert.h"

namespace tk {

image::Ptr convertImage(img::Image const & image)
{
	// TODO check if that can be optimized when image.bpp() == 32
	auto buf = image.toRawBits(32);
	return image::create(image::Params(image::Type::BGRA,WDims(image.width(),image.height()),&buf[0]));
}

}
