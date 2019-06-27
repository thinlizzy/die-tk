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

void ImageCanvasWin::drawImage(tk::image::Ptr const & image, tk::Point pos) {
	// log::info("ImageCanvasWin::drawImage at ",pos," src ",image::info(image)," target ",image::info(imageBuffer));
	// workaround to skip GdiAlphaBlend, since it does not work when drawing into an image canvas
//	if( auto * bitmapAlpha = dynamic_cast<image::BitmapAlpha *>(&*image) ) {
//		bitmapAlpha->drawTransparent(imageCanvas(),pos);
//	} else {
//		image->drawInto(imageCanvas(),pos);
//	}

	// TODO blend in a different way when both image and imageBuffer are transparent because the colors are changing a bit
	image->drawInto(imageCanvas(),pos);
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
			}
//			quad.rgbReserved = 255 - quad.rgbReserved;
		});
	} else {
		bitmap.replaceAllQuads([](RGBQUAD & quad) {
			quad.rgbReserved = 0;
		});
	}
	return imageBuffer;
}

}
