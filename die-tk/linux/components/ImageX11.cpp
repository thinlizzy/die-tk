#include "ImageX11.h"
#include <algorithm>
#include "../ResourceManager.h"
#include "../../components/NullImage.h"
#include "../../log.h"

namespace {

tk::ResourceManagerSingleton resourceManager;

size_t bitmapLineSize(int width) {
	auto lineSize = width / 8;
	if( width % 8 != 0 ) ++lineSize;
	return lineSize;
}

// nearest neighbor
int nnb(int d, int dLen, int sLen) {
	return d * sLen / dLen;
}

}

namespace tk {

namespace image {

char * duplicateBuffer(Params const & params) {
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
		default:
			log::error("image type not supported: ",static_cast<int>(params.type_));
		}
	} else {
		// image buffer default will have black transparent pixels
		std::fill(result,result+totalBytesDest,0);
	}
	return result;
}

// TODO add option to change the resize algorithm (nnb, bilinear interpolation, etc.)
char * imageResize(XImage const * imagePtr, WDims dims) {
	auto totalBytesDest = dims.area() * 4;
	char * result = static_cast<char *>(malloc(totalBytesDest));

	auto dst = result;
	int dx = 0;
	int dy = 0;
	int sy = 0;
	for( size_t p = 0; p < dims.area(); ++p ) {
		int sx = nnb(dx, dims.width, imagePtr->width);

		auto src = imagePtr->data + sy*imagePtr->bytes_per_line + sx*4;
		*dst++ = *src;
		*dst++ = *(src+1);
		*dst++ = *(src+2);
		*dst++ = *(src+3);

		++dx;
		if( dx == dims.width ) {
			dx = 0;
			++dy;
			sy = nnb(dy, dims.height, imagePtr->height);
		}
	}
	return result;
}

// TODO melhorar esse algoritmo
char * bitmapResize(XImage const * imagePtr, WDims dims) {
	auto lineSize = bitmapLineSize(imagePtr->width);
	char * result = new char[imagePtr->height * lineSize];
	std::fill_n(result,imagePtr->height * lineSize,0);

	auto dst = result;
	int dx = 0;
	int dy = 0;
	int sy = 0;
	for( size_t p = 0; p < dims.area(); ++p ) {
		int sx = nnb(dx, dims.width, imagePtr->width);
		auto sourceByte = imagePtr->data + sy*imagePtr->bytes_per_line + sx/8;
		auto hasSourceBit = bool(*sourceByte & (1 << (sx % 8)));
		if( hasSourceBit ) {
			*dst = *dst | (1 << (dx % 8));
		}

		++dx;
		if( dx == dims.width ) {
			dx = 0;
			++dy;
			sy = nnb(dy, dims.height, imagePtr->height);
		}
		if( dx % 8 == 0 ) {
			++dst;
		}
	}

	return result;
}

// image create()

Ptr create(Params const & params) {
	auto newBuf = duplicateBuffer(params);
	return params.tryTransparent_ ?
			createTransparentBGRA(params.dimensions_,newBuf) :
			createNativeBGRA(params.dimensions_,newBuf);
}

XImage * doCreateNativeBGRA(WDims dims, char * buffer) {
	auto imagePtr = XCreateImage(
		resourceManager->dpy,
		DefaultVisual(resourceManager->dpy,0),
		resourceManager->defaultDepth,
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

Ptr createNativeBGRA(WDims dims, char * buffer) {
	auto imagePtr = doCreateNativeBGRA(dims,buffer);
	if( ! imagePtr ) return nullImage;

	return std::make_shared<ImageX11>(imagePtr);
}

Ptr createTransparentBGRA(WDims dims, char * buffer) {
	auto imagePtr = doCreateNativeBGRA(dims,buffer);
	if( ! imagePtr ) return nullImage;

	return std::make_shared<ImageX11Transparent>(imagePtr);
}

Ptr createTransparentBGRA(WDims dims, std::vector<bool> const & transparentMask, char * buffer) {
	auto imagePtr = doCreateNativeBGRA(dims,buffer);
	if( ! imagePtr ) return nullImage;

	return std::make_shared<ImageX11Transparent>(imagePtr,transparentMask);
}

void drawImage(xImagePtr const & image, CanvasX11 & canvas, Rect srcrect, Point dest) {
	XPutImage(
		resourceManager->dpy,
		canvas.getDrawable(),
		canvas.getGC(),
		image.get(),
		srcrect.left, srcrect.top,
		dest.x, dest.y,
		srcrect.dims().width, srcrect.dims().height);
}

// *** ImageX11 *** //

ImageX11::ImageX11(XImage * imagePtr):
	xImage(imagePtr)
{
}

unsigned ImageX11::bpp() const {
	return xImage->bits_per_pixel;
}

WDims ImageX11::dims() const {
	return WDims(xImage->width,xImage->height);
}

CanvasX11 & ImageX11::beginDraw() {
	if( ! drawingArea ) {
		drawingArea.reset(XCreatePixmap(
			resourceManager->dpy,
			resourceManager->root(),
			xImage->width, xImage->height,
			xImage->depth
		));
		drawingCanvas = CanvasX11(drawingArea.get());
	}
	drawImage(xImage,drawingCanvas,Rect::closed(Point(0,0),dims()),Point(0,0));
	return drawingCanvas;
}

CanvasX11 & ImageX11::canvas() {
	return drawingCanvas;
}

void ImageX11::endDraw() {
	XGetSubImage(
		resourceManager->dpy,
		drawingArea.get(),
		0,0,
		xImage->width,xImage->height,
		-1,
		ZPixmap,
		xImage.get(),
		0,0);
}

void ImageX11::copyRectInto(CanvasX11 & canvas, Rect srcrect, Point dest) {
	// fit srcrect (and dest) in image dims
	if( srcrect.left < 0 ) {
		dest.x += -srcrect.left;
		srcrect.left = 0;
	}
	if( srcrect.top < 0 ) {
		dest.y += -srcrect.top;
		srcrect.top = 0;
	}
	if( srcrect.right > xImage->width ) {
		srcrect.right = xImage->width;
	}
	if( srcrect.bottom > xImage->height ) {
		srcrect.bottom = xImage->height;
	}

	drawImage(xImage,canvas,srcrect,dest);
}

void ImageX11::drawInto(CanvasX11 & canvas, Point dest) {
	drawImage(xImage,canvas,Rect::closed(Point(0,0),dims()),dest);
}

void ImageX11::drawInto(CanvasX11 & canvas, Rect destrect) {
	if( dims() == destrect.dims() ) {
		drawImage(xImage,canvas,Rect::closed(Point(0,0),destrect.dims()),destrect.topLeft());
	} else {
		auto imageBuffer = imageResize(xImage.get(), destrect.dims());
		auto xResizedImage = doCreateNativeBGRA(destrect.dims(), imageBuffer);
		if( ! xResizedImage ) return;

		auto resizedImgPtr = xImagePtr(xResizedImage);
		drawImage(resizedImgPtr,canvas,Rect::closed(Point(0,0),destrect.dims()),destrect.topLeft());
	}
}

void ImageX11::forEach(std::function<void(Bgra &)> callback) {
	auto p = xImage->data;
	for( auto h = 0; h != xImage->height; ++h ) {
		for( auto w = 0; w != xImage->width; ++w,p+=4 ) {
			callback(*reinterpret_cast<Bgra *>(p));
		}
	}
}

// ImageX11Transparent helpers

class ClipMaskGuard {
	GC gc;
public:
	ClipMaskGuard(CanvasX11 & canvas, Pixmap transparentMask, Point dest):
		gc(canvas.getGC())
	{
		XSetClipOrigin(resourceManager->dpy, gc, dest.x, dest.y);
		XSetClipMask(resourceManager->dpy, gc, transparentMask);
	}

	~ClipMaskGuard() {
		XSetClipMask(resourceManager->dpy, gc, None);
	}

	ClipMaskGuard(ClipMaskGuard const &) = delete;
};

std::unique_ptr<unsigned char[]> getTransparentMask(XImage * imagePtr) {
	auto lineSize = bitmapLineSize(imagePtr->width);
	unsigned char * result = new unsigned char[imagePtr->height * lineSize];

	auto src = imagePtr->data; // first pixel in the source image
	for( auto h = 0; h != imagePtr->height; ++h ) {
		auto dst = result + h*lineSize; // first pixel in the destination image
		*dst = 0;
		unsigned char currentBit = 0x1;
		for( auto w = 0; ; ) { // for each pixel in the line
			// opaque when alpha channel is not zero
			if( src[3] != 0 ) {
				*dst |= currentBit; // set the current bit when opaque
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

std::unique_ptr<unsigned char[]> getTransparentMask(XImage * imagePtr, std::vector<bool> const & transparentMask) {
	auto lineSize = bitmapLineSize(imagePtr->width);
	unsigned char * result = new unsigned char[imagePtr->height * lineSize];

	auto s = 0;
	for( auto h = 0; h != imagePtr->height; ++h ) {
		auto dst = result + h*lineSize; // first pixel in the destination image
		*dst = 0;
		unsigned char currentBit = 0x1;
		for( auto w = 0; ; ) { // for each pixel in the line
			if( ! transparentMask[s] ) {
				*dst |= currentBit; // set the current bit when opaque
			}

			++s; // next source pixel

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

::Pixmap createTransparentPixmap(XImage * imagePtr) {
	auto tpMaskPtr = getTransparentMask(imagePtr);
	return XCreateBitmapFromData(
		resourceManager->dpy,
		resourceManager->root(),
		reinterpret_cast<char *>(tpMaskPtr.get()),
		imagePtr->width, imagePtr->height);
}

// ImageX11Transparent //

ImageX11Transparent::ImageX11Transparent(XImage * imagePtr):
	ImageX11(imagePtr),
	transparentMask(createTransparentPixmap(imagePtr))
{
}

ImageX11Transparent::ImageX11Transparent(XImage * imagePtr, std::vector<bool> const & transparentMask):
	ImageX11(imagePtr),
	transparentMask(XCreateBitmapFromData(
		resourceManager->dpy,
		resourceManager->root(),
		reinterpret_cast<char *>(getTransparentMask(imagePtr,transparentMask).get()),
		imagePtr->width, imagePtr->height))
{
}

void ImageX11Transparent::endDraw() {
	ImageX11::endDraw();
	// creates the transparent mask again, assuming all 'touched' pixels don't have alpha == 0
	transparentMask.reset(createTransparentPixmap(xImage.get()));
}

void ImageX11Transparent::drawInto(CanvasX11 & canvas, Point dest) {
	ClipMaskGuard guard(canvas,transparentMask.get(),dest);
	ImageX11::drawInto(canvas,dest);
}

void ImageX11Transparent::drawInto(CanvasX11 & canvas, Rect destrect) {
	if( dims() == destrect.dims() ) {
		ClipMaskGuard guard(canvas,transparentMask.get(),destrect.topLeft());
		ImageX11::drawInto(canvas, destrect);
	} else {
		auto resizedMask = scoped::Pixmap(resizedTransparentMask(destrect.dims()));
		ClipMaskGuard guard(canvas,resizedMask.get(),destrect.topLeft());
		ImageX11::drawInto(canvas, destrect);
	}
}

Pixmap ImageX11Transparent::resizedTransparentMask(WDims newDims) {
	auto capturedImage = XGetImage(
		resourceManager->dpy,
		transparentMask.get(),
		0,0,
		xImage->width,xImage->height,
		-1,
		ZPixmap);
	if( capturedImage == 0 ) {
		log::error("failed to capture Pixmap");
	}
	auto resizedMask = std::unique_ptr<char[]>(bitmapResize(capturedImage,newDims));
	return XCreateBitmapFromData(
		resourceManager->dpy,
		resourceManager->root(),
		resizedMask.get(),
		newDims.width, newDims.height);
}

// TODO I suspect the destination point needs to be adjusted for srcrect in the clipmask - test and fix
void ImageX11Transparent::copyRectInto(CanvasX11 & canvas, Rect srcrect, Point dest) {
	ClipMaskGuard guard(canvas,transparentMask.get(),dest);
	ImageX11::copyRectInto(canvas,srcrect,dest);
}

std::ostream & operator<<(std::ostream & os, Bgra const & bgra) {
	os << '(' << int(bgra.b) << ',' << int(bgra.g) << ',' << int(bgra.r) << ',' << int(bgra.a) << ')';
	return os;
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
Pixmap pixmap(XCreateBitmapFromData(resourceManager->dpy, canvasX11.windowId,
			reinterpret_cast<char *>(transparentBuffer.get()),
			dims().width, dims().height));
canvasX11.setForegroundColor(RGBColor(255,255,255));
XCopyPlane(resourceManager->dpy, pixmap, canvasX11.windowId, canvasX11.gc,
		0, 0, dims().width, dims().height, dest.x, dest.y, 1);
XFreePixmap(resourceManager->dpy, pixmap);

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

