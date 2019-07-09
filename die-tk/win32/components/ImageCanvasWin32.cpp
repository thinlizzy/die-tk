#include "ImageCanvasWin32.h"
#include "ImageWin32.h"
#include "../../components/NullImage.h"

namespace tk {

std::shared_ptr<ImageCanvas> ImageCanvas::create(tk::WDims dims, bool transparent) {
	auto imageBuffer = tk::image::create(tk::image::Params(tk::image::Type::BGRA,dims).tryTransparent(transparent));
	if( transparent ) {
		dynamic_cast<image::BitmapAlpha &>(*imageBuffer).replaceAllQuads([](RGBQUAD & quad) {
			quad.rgbReserved = 255;
		});
	}
	return std::make_shared<ImageCanvasWin>(imageBuffer);
}

ImageCanvasWin::ImageCanvasWin(tk::image::Ptr imageBuffer):
	imageBuffer(std::move(imageBuffer)),
	canvasImpl(dynamic_cast<image::Bitmap &>(*this->imageBuffer).beginDraw())
{
}

ImageCanvasWin::~ImageCanvasWin() {
	// make sure there are no leaks
	imageBuffer->endDraw();
}

tk::Canvas & ImageCanvasWin::imageCanvas() {
	return canvasImpl;
}

tk::image::Ptr ImageCanvasWin::finishAndCreateImage() {
	imageBuffer->endDraw();

	auto & bitmap = dynamic_cast<image::Bitmap &>(*imageBuffer);
	if( typeid(bitmap) == typeid(image::BitmapAlpha) ) {
		// if the buffer was transparent, then need to make all touched pixels opaque and the untouched pixels transparent
		bitmap.replaceAllQuads([](RGBQUAD & quad) {
			switch(quad.rgbReserved) {
				case 0:
					// pixel was touched. make it opaque
					quad.rgbReserved = 255;
					break;
				case 255:
					// pixel was never touched. make it transparent
					quad.rgbReserved = 0;
					break;
//				default:
//					std::cout << int(quad.rgbReserved) << ',';
			}
		});
	} else {
		// non transparent images need to clean up all left over alpha values from previous blending operations
		bitmap.replaceAllQuads([](RGBQUAD & quad) {
			// unline line 46, 0 is the correct alpha value for non transparent images
			quad.rgbReserved = 0;
		});
	}
	return imageBuffer;
}

void ImageCanvasWin::drawImage(tk::image::Ptr const & image, tk::Point pos) {
	if( image == image::nullImage ) return;
	auto * srcBitmapAlpha = dynamic_cast<image::BitmapAlpha *>(&*image);
	if( srcBitmapAlpha && typeid(*imageBuffer) == typeid(image::BitmapAlpha) ) {
		srcBitmapAlpha->drawTonT(canvasImpl,pos);
	} else {
		dynamic_cast<image::ImageImpl &>(*image).drawInto(canvasImpl,pos);
	}
}

void ImageCanvasWin::drawImage(tk::image::Ptr const & image, Rect destrect) {
	if( image == image::nullImage ) return;
	dynamic_cast<image::ImageImpl &>(*image).drawInto(canvasImpl,destrect);
}

void ImageCanvasWin::copyRectImage(tk::image::Ptr const & image, Rect srcrect, Point dest) {
	if( image == image::nullImage ) return;
	dynamic_cast<image::ImageImpl &>(*image).copyRectInto(canvasImpl,srcrect,dest);
}

}
