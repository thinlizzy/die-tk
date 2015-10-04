#include "BallView.h"
#include <iostream>

BallView::BallView(Ball & ball, tk::Window & window):
	ball(ball),
	window(window),
	ballColor1(0,255,0),
	ballColor2(0,200,0),
	backgroundColor(0,0,0)
{
}

void BallView::moveBall(tk::Point offset)
{
	auto rect = ball.rect().add(-offset); // get previous position rectangle
	// erase X movement
	if( offset.x > 0 ) {
		window.canvas().fillRect(rect.resize(rect.dims().setWidth(offset.x+drawOff)),backgroundColor);
	} else
	if( offset.x < 0 ) {
		window.canvas().fillRect(rect.resizeBottomRight(rect.dims().setWidth(-offset.x+drawOff)),backgroundColor);
	}
	// erase Y movement
	if( offset.y > 0 ) {
		window.canvas().fillRect(rect.resize(rect.dims().setHeight(offset.y+drawOff)),backgroundColor);
	} else
	if( offset.y < 0 ) {
		window.canvas().fillRect(rect.resizeBottomRight(rect.dims().setHeight(-offset.y+drawOff)),backgroundColor);
	}
	draw();
}

void BallView::draw()
{
	auto rect = ball.rect();
	auto dims = rect.dims();
	window.canvas().fillRect(rect.resize(dims - tk::WDims::squared(drawOff)).add(tk::Point(drawOff/2,drawOff/2)), ballColor2); // inner
	window.canvas().fillRect(rect.resize(dims.addHeight(-drawOff*2)).add(tk::Point().addY(drawOff)), ballColor1); // wide
	window.canvas().fillRect(rect.resize(dims.addWidth(-drawOff*2)).add(tk::Point().addX(drawOff)), ballColor1); // tall
}

void BallView::drawIfInRect(tk::Rect rect)
{
	if( ball.rect().intersect(rect) ) {
		draw();
	}
}
