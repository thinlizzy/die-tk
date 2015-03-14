#include "ImageX11.h"
#include "../ResourceManager.h"
#include "../../NullCanvas.h"
#include "../../components/NullImage.h"
#include "../../log.h"
#include <algorithm>

namespace tk {

namespace image {

char * duplicateBuffer(Params const & params)
{
	auto totalBytesDest = params.dimensions_.area() * 4;
	char * result = static_cast<char *>(malloc(totalBytesDest));
	if( params.buffer_ ) {
		auto src = params.buffer_;
		auto dst = result;
		switch( params.type_ ) {
		case Type::BGRA:
			std::copy(src,src+totalBytesDest,dst);
			break;
		case Type::RGBA:
			for( size_t p = 0; p < params.dimensions_.area(); ++p ) {
				*dst++ = *(src+2);
				*dst++ = *(src+1);
				*dst++ = *src;
				*dst++ = *(src+3);
				src += 4;
			}
			break;
		case Type::RGB:
			for( size_t p = 0; p < params.dimensions_.area(); ++p ) {
				*dst++ = *(src+2);
				*dst++ = *(src+1);
				*dst++ = *src;
				*dst++ = 0;
				src += 3;
			}
			break;
		case Type::BGR:
			for( size_t p = 0; p < params.dimensions_.area(); ++p ) {
				*dst++ = *src++;
				*dst++ = *src++;
				*dst++ = *src++;
				*dst++ = 0;
			}
			break;
		case Type::gray:
			for( size_t p = 0; p < params.dimensions_.area(); ++p ) {
				*dst++ = *src;
				*dst++ = *src;
				*dst++ = *src;
				*dst++ = 0;
				++src;
			}
			break;
		}
	}
	return result;
}

Ptr create(Params const & params)
{
	auto newBuf = duplicateBuffer(params);
	auto imagePtr = XCreateImage(
		resourceManager.dpy,
		DefaultVisual(resourceManager.dpy,0),
		24, // params.bpp(),
		ZPixmap,
		0,
		newBuf,
		params.dimensions_.width,
		params.dimensions_.height,
		32,
		0);
	if (imagePtr) {
		return std::make_shared<ImageX11>(imagePtr);
	}

	log::error("Failed to create XImage");
	free(newBuf);
	return nullImage;
	// not using external header anymore. will create a lib that adapts free image loading
	// perhaps create a RGBA type and move tryTransparent logic to this lib
}

ImageX11::ImageX11(XImage * imagePtr):
	xImage(imagePtr)
{
}

unsigned ImageX11::bpp() const
{
	return xImage->bits_per_pixel;
}

WDims ImageX11::dims() const
{
	return WDims(xImage->width,xImage->height);
}

Canvas & ImageX11::beginDraw()
{
	// TODO copy to a pixmap and return a canvas to it
	return nullCanvas;
}

Canvas & ImageX11::canvas()
{
	// TODO
	return nullCanvas;
}

void ImageX11::endDraw()
{
	// TODO copy the pixmap back into the image and delete it
}

void ImageX11::drawInto(Canvas & canvas, Point dest)
{
	auto & canvasX11 = static_cast<CanvasX11 &>(canvas);
	XPutImage(
		resourceManager.dpy,
		canvasX11.windowId,
		canvasX11.gc,
		xImage.get(),
		0, 0,
		dest.x, dest.y,
		xImage->width, xImage->height);
}

void ImageX11::drawInto(Canvas & canvas, Rect destrect)
{
	// TODO needs to rescale the image to the destrect. this is doing a wrong copyrect, instead
	auto & canvasX11 = static_cast<CanvasX11 &>(canvas);
	XPutImage(
		resourceManager.dpy,
		canvasX11.windowId,
		canvasX11.gc,
		xImage.get(),
		0, 0,
		destrect.left, destrect.top,
		destrect.width(), destrect.height());
}

}

}
