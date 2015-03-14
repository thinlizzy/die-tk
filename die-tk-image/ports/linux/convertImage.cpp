#include "../../convert.h"
#include "../src/linux/components/ImageX11.h"

namespace tk {

image::Ptr convertImage(img::Image const & image)
{
	// TODO check if that can be optimized when image.bpp() == 32
	auto buf = image.toRawBits(32);
	return image::createAndOwnBGRA(
			WDims(image.width(),image.height()),
			reinterpret_cast<char *>(buf.release()));
}

}
