#include "ImageCanvasWin32.h"
#include "ImageWin32.h"
#include "../../log.h"

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
	canvas(this->imageBuffer->beginDraw())
{
}

ImageCanvasWin::~ImageCanvasWin() {
	// make sure there are no leaks
	imageBuffer->endDraw();
}

tk::Canvas & ImageCanvasWin::imageCanvas() {
	return canvas;
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
	auto * srcBitmapAlpha = dynamic_cast<image::BitmapAlpha *>(&*image);
	if( srcBitmapAlpha && typeid(*imageBuffer) == typeid(image::BitmapAlpha) ) {
		srcBitmapAlpha->drawTonT(imageCanvas(),pos);
	} else {
		ImageCanvas::drawImage(image,pos);
	}
}

}
