#include "CanvasX11.h"
#include "ResourceManager.h"
#include <array>
#include <memory>
#include <type_traits>
#include <vector>
#include <algorithm>
#include <iterator>
#include <X11/Xutil.h>
#include "../log.h"

namespace tk {

XPoint toXPoint(Point point)
{
	return { point.x, point.y };
}

std::array<XPoint,4> rectToPoints(const Rect & rect)
{
	return {
		toXPoint(rect.topLeft()),
		toXPoint(rect.topRight()),
		toXPoint(rect.bottomRight()),
		toXPoint(rect.bottomLeft()),
	};
}

/*
short to16BitColor(char c) { return c * 65535 / 255; }

XColor toXColor(const RGBColor & color)
{
	XColor result;
	result.blue = to16BitColor(color.b);
	result.red = to16BitColor(color.r);
	result.green = to16BitColor(color.g);
	return result;
}
*/

unsigned long rgb32(RGBColor const & color)
{
	return (color.r << 16) | (color.g << 8) | color.b;
}

//

CanvasX11::CanvasX11():
	gc(nullptr)
{
}

CanvasX11::CanvasX11(::Window windowId):
	gc(XCreateGC(resourceManager.dpy,windowId,0,nullptr)),
	windowId(windowId)
{
}

CanvasX11::CanvasX11(CanvasX11 && other):
	gc(other.gc),
    windowId(other.windowId)
{
	other.gc = nullptr;
}

CanvasX11 & CanvasX11::operator=(CanvasX11 && other)
{
	if( this != &other ) {
		gc = other.gc;
	    windowId = other.windowId;
		other.gc = nullptr;
	}

	return *this;
}

CanvasX11::~CanvasX11()
{
	if( gc ) {
		XFreeGC(resourceManager.dpy,gc);
	}
}

void CanvasX11::translate(Point p)
{
	XSetClipOrigin(resourceManager.dpy,gc,p.x,p.y);
}

void CanvasX11::clearTranslate()
{
	translate(Point(0,0));
}

void CanvasX11::addClipRect(const Rect & openrect)
{
	// TODO need to make it to add a new rectangle, instead of setting only one rectangle

	auto points = rectToPoints(openrect);
	auto region = XPolygonRegion(points.data(),4,EvenOddRule);
	auto regionPtr = std::unique_ptr<std::remove_reference<decltype(*region)>::type,int (*)(Region)>(
			region,&XDestroyRegion);

	XSetRegion(resourceManager.dpy,gc,region);
}

void CanvasX11::clearClipping()
{
	XSetClipMask(resourceManager.dpy,gc,None);
}

void CanvasX11::setForegroundColor(RGBColor const & color)
{
	XSetForeground(resourceManager.dpy,gc,rgb32(color));
	/*
	auto xColor = toXColor(color);
	auto colormap = DefaultColormap(resourceManager.dpy, DefaultScreen(resourceManager.dpy));
	XAllocColor(resourceManager.dpy,colormap,&xColor);
	XSetForeground(resourceManager.dpy,gc,xColor.pixel);
	*/
}

void CanvasX11::setPen(Pen const & pen)
{
	char dashList[] = {4,4};
	char dotList[] = {1,1};
	char dashDotList[] = {4,1,1,1};
	char dashDotDotList[] = {4,1,1,1,1,1};

	setForegroundColor(pen.color);

	int lineStyle = LineOnOffDash;
	switch(pen.style) {
	case PenStyle::solid:
		lineStyle = LineSolid;
		break;
	case PenStyle::dash:
		XSetDashes(resourceManager.dpy,gc,0,dashList,sizeof(dashList));
		break;
	case PenStyle::dot:
		XSetDashes(resourceManager.dpy,gc,0,dashList,sizeof(dotList));
		break;
	case PenStyle::dashdot:
		XSetDashes(resourceManager.dpy,gc,0,dashList,sizeof(dashDotList));
		break;
	case PenStyle::dashdotdot:
		XSetDashes(resourceManager.dpy,gc,0,dashList,sizeof(dashDotDotList));
		break;
	// TODO PenStyle::invisible not implemented
	}
	XSetLineAttributes(resourceManager.dpy,gc,pen.width,lineStyle,CapButt,JoinMiter);
}

void CanvasX11::setBrush(Brush const & brush)
{
	setForegroundColor(brush.color);

	// TODO set brush styles
}

void CanvasX11::plot(Point p, const RGBColor & color)
{
	setForegroundColor(color);
	XDrawPoint(resourceManager.dpy,windowId,gc,p.x,p.y);
}

void CanvasX11::drawLine(Point p1, Point p2, const Pen & pen)
{
	setPen(pen);
	XDrawLine(resourceManager.dpy,windowId,gc,p1.x,p1.y,p2.x,p2.y);
}

void CanvasX11::drawPoly(const Points & polygon, const Pen & pen)
{
	if( polygon.size() < 3 ) return;

	setPen(pen);

	std::vector<XPoint> points;
	std::transform(polygon.begin(),polygon.end(),std::back_inserter(points),[](auto p){
		return XPoint{p.x, p.y};
	});
	points.push_back({polygon.front().x,polygon.front().y});
	XDrawLines(resourceManager.dpy,windowId,gc,&points[0],polygon.size()+1,CoordModeOrigin);
}

void CanvasX11::rectangle(const Rect & rect, const Pen & pen)
{
	setPen(pen);
	XDrawRectangle(resourceManager.dpy,windowId,gc,rect.left,rect.top,rect.width(),rect.height());
}

void CanvasX11::fillRect(const Rect & openrect, const Brush & brush)
{
	setBrush(brush);
	XFillRectangle(resourceManager.dpy,windowId,gc,openrect.left,openrect.top,openrect.width(),openrect.height());
}

void CanvasX11::drawText(Point p, die::NativeString const & text, RGBColor const & color)
{
	setForegroundColor(color);
	auto textDims = measureText(text);
	XDrawString(resourceManager.dpy,windowId,gc,p.x,p.y+textDims.height,text.str.data(),text.str.size());
}

void CanvasX11::textRect(const Rect & openrect, const die::NativeString & text, const TextParams & params)
{
	fillRect(openrect,params.backgroundColor);

	// TODO save current clipping rects. may require local caching of regions due to http://tronche.com/gui/x/xlib/GC/XGetGCValues.html
	addClipRect(openrect);

	int x,y;
	auto textDims = measureText(text);
	switch( params.h_align ) {
	case HTextAlign::left:
		x = openrect.left;
		break;
	case HTextAlign::right:
		x = openrect.right - textDims.width;
		break;
	case HTextAlign::center:
		x = (openrect.right + openrect.left) / 2 - textDims.width / 2;
		break;
	}
	switch( params.v_align ) {
	case VTextAlign::top:
		y = openrect.top;
		break;
	case VTextAlign::bottom:
		y = openrect.bottom - textDims.height;
		break;
	case VTextAlign::center:
		y = (openrect.bottom + openrect.top) / 2 - textDims.height / 2;
		break;
	}

	setForegroundColor(params.textColor);
	XDrawString(resourceManager.dpy,windowId,gc,x,y+textDims.height,text.str.data(),text.str.size());

	// TODO restore clipping rects instead
	clearClipping();
}

WDims CanvasX11::measureText(const die::NativeString & text)
{
	WDims result;
	auto gid = XGContextFromGC(gc);

	// TODO cache the font on gc creation
	auto fontStruct = XQueryFont(resourceManager.dpy,gid);
	if( fontStruct == NULL ) {
		log::error("measureText ","can't get font info for gc ",gc);
		return result;
	}
	auto fontStructGuard = std::unique_ptr<std::remove_reference<decltype(*fontStruct)>::type,
			void (*)(XFontStruct *)>(fontStruct,[](XFontStruct * ptr) {
		XFreeFontInfo(NULL,ptr,1);
	});

	int direction, font_ascent, font_descent;
	XCharStruct overall;
	XTextExtents(fontStruct,text.str.data(),text.str.size(),
			&direction, &font_ascent, &font_descent, &overall);

	result.width = overall.width;
	result.height = overall.ascent; // or font_ascent - font_descent ?
//	result.height = font_ascent;

		// XQueryTextExtents(resourceManager.dpy, gid, text.str.data(),text.str.size(),&direction, &font_ascent, &font_descent, &overall);

	return result;
}

void CanvasX11::draw(image::Ptr image, Point dest)
{
	throw "CanvasX11::draw not implemented";
}

void CanvasX11::draw(image::Ptr image, Rect destrect)
{
	throw "CanvasX11::drawRect not implemented";
}

}