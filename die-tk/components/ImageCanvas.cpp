#include "ImageCanvas.h"

namespace tk {

void ImageCanvas::translate(Point p) {
	imageCanvas().translate(p);
}

void ImageCanvas::clearTranslate() {
	imageCanvas().clearTranslate();
}

void ImageCanvas::addClipRect(Rect const & openrect) {
	imageCanvas().addClipRect(openrect);
}

void ImageCanvas::clearClipping() {
	imageCanvas().clearClipping();
}

void ImageCanvas::plot(Point p, RGBColor const & color) {
	imageCanvas().plot(p,color);
}

void ImageCanvas::drawLine(Point p1, Point p2, Pen const & pen) {
	imageCanvas().drawLine(p1,p2,pen);
}

void ImageCanvas::drawPoly(Points const & polygon, Pen const & pen) {
	imageCanvas().drawPoly(polygon,pen);
}

void ImageCanvas::rectangle(Rect const & rect, Pen const & pen) {
	imageCanvas().rectangle(rect,pen);
}

void ImageCanvas::fillRect(Rect const & openrect, Brush const & brush) {
	imageCanvas().fillRect(openrect,brush);
}

void ImageCanvas::roundRect(Rect const & openrect, WDims ellipseDims, Pen const & pen, Brush const & brush) {
	imageCanvas().roundRect(openrect,ellipseDims,pen,brush);
}

void ImageCanvas::drawText(Point p, NativeString const & text, RGBColor const & color) {
	imageCanvas().drawText(p,text,color);
}

void ImageCanvas::drawText(Point p, NativeString const & text, RGBColor const & textColor, RGBColor const & backgroundColor) {
	imageCanvas().drawText(p,text,textColor,backgroundColor);
}

void ImageCanvas::textRect(Rect const & openrect, NativeString const & text, const TextParams & params) {
	imageCanvas().textRect(openrect,text,params);
}

WDims ImageCanvas::measureText(NativeString const & text) {
	return imageCanvas().measureText(text);
}

}
