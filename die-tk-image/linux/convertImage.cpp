#include "../src/convert.h"
#include "../linux/die-tk-linux.h"
#include <vector>

namespace tk {

std::vector<bool> getTransparentMask(img::Image const & image)
{
	auto dims = WDims(image.width(),image.height());
	std::vector<bool> transparentMask(dims.area());
	img::Size x = 0;
	img::Size y = dims.height - 1;
	for( int d = 0; d < dims.area(); ++d ) {
		transparentMask[d] = image.isTransparentPixel(x,y);
		++x;
		if( x == dims.width ) {
			x = 0;
			--y;
		}
	}
	return transparentMask;
}

image::Ptr convertImage(img::Image const & image)
{
	// TODO check if toRawBits() can be optimized when image.bpp() == 32

	auto dims = WDims(image.width(),image.height());
	if( image.getTransparentColorIndex() >= 0 ) {
		auto buf = image.toRawBits(32);
		return image::createTransparentBGRA(dims,
				getTransparentMask(image),
				reinterpret_cast<char *>(buf.release()));
	} else
	if( image.transparent() ) {
		auto buf = image.toRawBits(32);
		return image::createTransparentBGRA(dims,
				reinterpret_cast<char *>(buf.release()));
	} else
	{
		auto buf = image.toRawBits(32);
		return image::createNativeBGRA(dims,
				reinterpret_cast<char *>(buf.release()));
	}
}

}
