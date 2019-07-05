#include "Painting.h"

Painting::Painting(tk::WDims dims):
	drawing(false),
	imgCanvas(tk::image::create(tk::image::Params().dimensions(dims))),
	pen(tk::RGBColor(200,0,0))
{
}

void Painting::setCrosshair(tk::Point point) {
	if( drawing ) {
		auto & canvas = imgCanvas->beginDraw();
		canvas.drawLine(coordinates,point,pen);
		imgCanvas->endDraw();
	}
	coordinates = point;
}

void Painting::penDown() {
	drawing = true;
}

void Painting::penUp() {
	drawing = false;
}

// resize to a bigger dimension if able
void Painting::resize(tk::WDims dims) {
	auto prevDims = imgCanvas->dims();
	if( dims.width < prevDims.width ) dims.width = prevDims.width;
	if( dims.height < prevDims.height ) dims.height = prevDims.height;
	if( prevDims == dims ) return;

	auto newImg = tk::image::create(tk::image::Params().dimensions(dims));
	// copy contents of previous image
	auto & canvas = newImg->beginDraw();
	canvas.drawImage(imgCanvas,tk::Point(0,0));
	newImg->endDraw();
	// replace the image with the resized one
	imgCanvas = newImg;
}
