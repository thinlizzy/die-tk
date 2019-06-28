#include "ImageCanvasX11.h"

namespace tk {

std::shared_ptr<ImageCanvas> ImageCanvas::create(tk::WDims dims, bool transparent) {
	auto imageBuffer = tk::image::create(tk::image::Params(tk::image::Type::BGRA,dims).tryTransparent(transparent));
	// preprocessing pixels?
	return std::make_shared<ImageCanvasX11>(imageBuffer);
}

ImageCanvasX11::ImageCanvasX11(tk::image::Ptr imageBuffer):
	imageBuffer(std::move(imageBuffer)),
	canvas(this->imageBuffer->beginDraw())
{
}

tk::Canvas & ImageCanvasX11::imageCanvas() {
	return canvas;
}

tk::image::Ptr ImageCanvasX11::finishAndCreateImage() {
	imageBuffer->endDraw();
	// postprocessing pixels?
	return imageBuffer;
}

}
