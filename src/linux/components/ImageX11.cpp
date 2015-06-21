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
				*dst++ = 255;
				src += 3;
			}
			break;
		case Type::BGR:
			for( size_t p = 0; p < params.dimensions_.area(); ++p ) {
				*dst++ = *src++;
				*dst++ = *src++;
				*dst++ = *src++;
				*dst++ = 255;
			}
			break;
		case Type::gray:
			for( size_t p = 0; p < params.dimensions_.area(); ++p ) {
				*dst++ = *src;
				*dst++ = *src;
				*dst++ = *src;
				*dst++ = 255;
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
	return params.tryTransparent_ ?
			createTransparentBGRA(params.dimensions_,newBuf) :
			createNativeBGRA(params.dimensions_,newBuf);
}

XImage * doCreateNativeBGRA(WDims dims, char * buffer)
{
	auto imagePtr = XCreateImage(
		resourceManager.dpy,
		DefaultVisual(resourceManager.dpy,0),
		24,
		ZPixmap,
		0,
		buffer,
		dims.width,
		dims.height,
		32,
		0);
	if( ! imagePtr ) {
		free(buffer);
		log::error("Failed to create XImage");
	}

	return imagePtr;
}

Ptr createNativeBGRA(WDims dims, char * buffer)
{
	auto imagePtr = doCreateNativeBGRA(dims,buffer);
	if( ! imagePtr ) return nullImage;

	return std::make_shared<ImageX11>(imagePtr);
}

Ptr createTransparentBGRA(WDims dims, char * buffer)
{
	auto imagePtr = doCreateNativeBGRA(dims,buffer);
	if( ! imagePtr ) return nullImage;

	return std::make_shared<ImageX11Transparent>(imagePtr);
}


// *** ImageX11 *** //

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
	if( ! drawingArea ) {
		drawingArea.reset(XCreatePixmap(
			resourceManager.dpy,
			resourceManager.root(),
			xImage->width, xImage->height,
			xImage->depth
		));
		drawingCanvas = CanvasX11(drawingArea.get());
	}
	drawInto(drawingCanvas,Point(0,0));
	return drawingCanvas;
}

Canvas & ImageX11::canvas()
{
	return drawingCanvas;
}

void ImageX11::endDraw()
{
	xImage.reset(XGetImage(
		resourceManager.dpy,
		drawingArea.get(),
		0,0,
		xImage->width, xImage->height,
		-1,
		ZPixmap
	));
}

void ImageX11::drawInto(Canvas & canvas, Point dest)
{
	if( &canvas == &nullCanvas ) return;

	auto & canvasX11 = static_cast<CanvasX11 &>(canvas);
	XPutImage(
		resourceManager.dpy,
		canvasX11.drawable,
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
		canvasX11.drawable,
		canvasX11.gc,
		xImage.get(),
		0, 0,
		destrect.left, destrect.top,
		destrect.width(), destrect.height());
}

// ImageX11Transparent helpers

class ClipMaskGuard {
	GC gc;
public:
	ClipMaskGuard(Canvas & canvas, Pixmap transparentMask, Point dest):
		gc(static_cast<CanvasX11 &>(canvas).gc)
	{
		XSetClipOrigin(resourceManager.dpy, gc, dest.x, dest.y);
		XSetClipMask(resourceManager.dpy, gc, transparentMask);
	}

	~ClipMaskGuard() {
		XSetClipMask(resourceManager.dpy, gc, None);
	}

	ClipMaskGuard(ClipMaskGuard const &) = delete;
};

size_t bitmapLineSize(int width)
{
	auto lineSize = width / 8;
	if( width % 8 != 0 ) ++lineSize;
	return lineSize;
}

std::unique_ptr<unsigned char[]> getTransparentMask(XImage * imagePtr)
{
	auto lineSize = bitmapLineSize(imagePtr->width);
	unsigned char * result = new unsigned char[imagePtr->height * lineSize];

	auto src = imagePtr->data; // first pixel in the source image
	for( auto h = 0; h != imagePtr->height; ++h ) {
		auto dst = result + h*lineSize; // first pixel in the destination image
		*dst = 0;
		unsigned char currentBit = 0x1;
		for( auto w = 0; ; ) { // for each pixel in the line
			if( src[3] != 0 ) {
				*dst |= currentBit; // set the current bit when alpha channel is not zero
			}

			src += 4; // next source pixel

			++w;
			if( w == imagePtr->width ) break;

			// next dest pixel
			if( currentBit == 0x80 ) {
				// bit is at the end of the byte, move to the next byte
				currentBit = 0x1;
				++dst;
				*dst = 0;
			} else {
				currentBit <<= 1;
			}
		}
	}
	return std::unique_ptr<unsigned char[]>(result);
}

// ImageX11Transparent //

ImageX11Transparent::ImageX11Transparent(XImage * imagePtr):
	ImageX11(imagePtr),
	transparentMask(XCreateBitmapFromData(resourceManager.dpy,
			resourceManager.root(),
			reinterpret_cast<char *>(getTransparentMask(imagePtr).get()),
			imagePtr->width, imagePtr->height))
{
}

void ImageX11Transparent::drawInto(Canvas & canvas, Point dest)
{
	ClipMaskGuard guard(canvas,transparentMask.get(),dest);
	ImageX11::drawInto(canvas,dest);
}

void ImageX11Transparent::drawInto(Canvas & canvas, Rect destrect)
{
	ClipMaskGuard guard(canvas,transparentMask.get(),destrect.topLeft());
	ImageX11::drawInto(canvas,destrect);
}

}

}

/*
#include <iostream>
#include <iomanip>

void debugPixmap(unsigned char * buf, int width, int height)
{
	if( width > 32 ) return;

	using namespace std;
	for( auto h = 0; h != height; ++h ) {
		for( auto w = 0; w < width; w += 8 ) {
			cout << hex << setw(2) << setfill('0') << unsigned(*buf);
			++buf;
		}
		cout << endl;
	}
}

debugPixmap(transparentBuffer.get(),dims().width, dims().height);
auto & canvasX11 = static_cast<CanvasX11 &>(canvas);
Pixmap pixmap(XCreateBitmapFromData(resourceManager.dpy, canvasX11.windowId,
			reinterpret_cast<char *>(transparentBuffer.get()),
			dims().width, dims().height));
canvasX11.setForegroundColor(RGBColor(255,255,255));
XCopyPlane(resourceManager.dpy, pixmap, canvasX11.windowId, canvasX11.gc,
		0, 0, dims().width, dims().height, dest.x, dest.y, 1);
XFreePixmap(resourceManager.dpy, pixmap);

ImageX11::drawInto(canvas, dest.addX(dims().width));
ImageX11::drawInto(canvas, dest.addY(dims().height));
*/

// LINKS

// http://tronche.com/gui/x/xlib/utilities/manipulating-bitmaps.html
// http://tronche.com/gui/x/xlib/utilities/manipulating-images.html
// http://tronche.com/gui/x/xlib/pixmap-and-cursor/pixmap.html

// http://www.linuxquestions.org/questions/programming-9/how-to-draw-color-images-with-xlib-339366/page2.html
// http://stackoverflow.com/questions/6384987/load-image-onto-a-window-using-xlib
// http://stackoverflow.com/questions/10513367/xlib-draw-only-set-bits-of-a-bitmap-on-a-window

