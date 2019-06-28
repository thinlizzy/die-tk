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
		bitmap.replaceAllQuads([](RGBQUAD & quad) {
			switch(quad.rgbReserved) {
				case 0:
					quad.rgbReserved = 255;
					break;
				case 255:
					quad.rgbReserved = 0;
					break;
				default:
					std::cout << int(quad.rgbReserved) << ',';
					// quad.rgbReserved = 0;
			}
		});
	} else {
		// non transparent images need to clean up all left over alpha values from previous blending operations
		bitmap.replaceAllQuads([](RGBQUAD & quad) {
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
