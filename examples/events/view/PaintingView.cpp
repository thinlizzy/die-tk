#include "PaintingView.h"

PaintingView::PaintingView(Painting & painting, tk::Window & window):
	painting(painting),
	window(window)
{
}

// it may have some flickering. double buffering implementation is left as an exercise for the reader :)

void PaintingView::drawCrosshair(tk::Point newPoint) {
	auto & canvas = window.canvas();
	canvas.drawLine(newPoint.addY(-crossHairRadius),newPoint.addY(+crossHairRadius),crossHairPen);
	canvas.drawLine(newPoint.addX(-crossHairRadius),newPoint.addX(+crossHairRadius),crossHairPen);
}

void PaintingView::drawX(tk::Point pos) {
	int off = 3;
	auto & canvas = window.canvas();
	canvas.drawLine(pos + tk::Point(-crossHairRadius,-crossHairRadius),pos + tk::Point(-off,-off),crossHairPen);
	canvas.drawLine(pos + tk::Point(+crossHairRadius,+crossHairRadius),pos + tk::Point(+off,+off),crossHairPen);
	canvas.drawLine(pos + tk::Point(+crossHairRadius,-crossHairRadius),pos + tk::Point(+off,-off),crossHairPen);
	canvas.drawLine(pos + tk::Point(-crossHairRadius,+crossHairRadius),pos + tk::Point(-off,+off),crossHairPen);
}

void PaintingView::move(tk::Point oldPoint, tk::Point newPoint) {
	eraseCrosshair(oldPoint);
	if( painting.isDrawing() ) {
		auto & canvas = window.canvas();
		canvas.drawLine(oldPoint,newPoint,painting.drawingPen());
		drawX(newPoint);
	} else {
		drawCrosshair(newPoint);
	}
}

void PaintingView::updatePen() {
	auto pos = painting.posCrosshair();
	eraseCrosshair(pos);
	if( painting.isDrawing() ) {
		drawX(pos);
	} else {
		drawCrosshair(pos);
	}
}

void PaintingView::eraseCrosshair(tk::Point oldPoint) {
	auto pos = oldPoint - tk::Point(crossHairRadius,crossHairRadius);
	auto rect = tk::Rect::open(pos,tk::WDims::squared(crossHairRadius*2));
	window.canvas().copyRectImage(painting.imageCanvas(),rect,pos);
}

void PaintingView::redraw(tk::Canvas & canvas, tk::Rect rect) {
	// not redrawing the cursor. not sure if it is needed
	canvas.copyRectImage(painting.imageCanvas(),rect,rect.topLeft());
}
