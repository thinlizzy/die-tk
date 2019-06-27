#include "ImageCanvasWin32.h"
#include "ImageWin32.h"

namespace tk {

std::shared_ptr<ImageCanvas> ImageCanvas::create(tk::WDims dims, bool transparent) {
	auto imageBuffer = tk::image::create(tk::image::Params(tk::image::Type::BGRA,dims).tryTransparent(transparent));
	auto & bitmap = dynamic_cast<image::Bitmap &>(*imageBuffer);
	bitmap.replaceAllQuads([](RGBQUAD & quad) { quad.rgbReserved = 255; });
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
	bitmap.replaceAllQuads([](RGBQUAD & quad) { if( quad.rgbReserved == 0 ) quad.rgbReserved = 255; });
	return imageBuffer;
}

}
