#include "ImageCanvasX11.h"
#include "ImageX11.h"

namespace tk {

std::shared_ptr<ImageCanvas> ImageCanvas::create(tk::WDims dims, bool transparent) {
	auto imageBuffer = tk::image::create(tk::image::Params(tk::image::Type::BGRA,dims).tryTransparent(transparent));
	return std::make_shared<ImageCanvasX11>(imageBuffer);
}

ImageCanvasX11::ImageCanvasX11(tk::image::Ptr imageBuffer):
	imageBuffer(std::move(imageBuffer)),
	canvas(dynamic_cast<image::ImageX11 &>(*this->imageBuffer).beginDraw())
{
}

tk::Canvas & ImageCanvasX11::imageCanvas() {
	return canvas;
}

tk::image::Ptr ImageCanvasX11::finishAndCreateImage() {
	imageBuffer->endDraw();
	return imageBuffer;
}

void ImageCanvasX11::drawImage(tk::image::Ptr const & image, tk::Point pos) {
	dynamic_cast<image::ImageX11 &>(*image).drawInto(canvas,pos);
}

void ImageCanvasX11::drawImage(tk::image::Ptr const & image, Rect destrect) {
	dynamic_cast<image::ImageX11 &>(*image).drawInto(canvas,destrect);
}

void ImageCanvasX11::copyRectImage(tk::image::Ptr const & image, Rect srcrect, Point dest) {
	dynamic_cast<image::ImageX11 &>(*image).copyRectInto(canvas,srcrect,dest);
}

}
