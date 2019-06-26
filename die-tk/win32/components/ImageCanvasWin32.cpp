#include "ImageCanvasWin32.h"
#include "ImageWin32.h"

namespace tk {

std::shared_ptr<ImageCanvas> ImageCanvas::create(tk::WDims dims, bool transparent) {
	auto imageBuffer = tk::image::create(tk::image::Params(tk::image::Type::BGRA,dims).tryTransparent(transparent));
	if( transparent ) {
		auto & bitmap = dynamic_cast<image::BitmapAlpha &>(*imageBuffer);
		bitmap.replaceAllQuads([](RGBQUAD & quad) { quad.rgbReserved = 255; });
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
	if( auto * bitmap = dynamic_cast<image::BitmapAlpha *>(&*imageBuffer) ) {
		bitmap->replaceAllQuads([](RGBQUAD & quad) { quad.rgbReserved = 255 - quad.rgbReserved; });
	}
	return imageBuffer;
}

}
