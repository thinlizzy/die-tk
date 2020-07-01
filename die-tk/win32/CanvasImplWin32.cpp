#include "CanvasImplWin32.h"
#include "ConvertersWin32.h"
#include "../log.h"
#include "../components/NullImage.h"
#include "components/ImageWin32.h"

namespace tk {

CanvasImpl::CanvasImpl():
    hpenOrig(0),
    hbrushOrig(0),
	dc(0)
{}

CanvasImpl::CanvasImpl(HDC dc):
    hpenOrig(0),
    hbrushOrig(0),
	dc(dc)
{}

CanvasImpl::CanvasImpl(CanvasImpl && temp):
    hpenOrig(temp.hpenOrig),
    hbrushOrig(temp.hbrushOrig),
	dc(temp.dc)
{
    temp.hpenOrig = 0;
    temp.hbrushOrig = 0;
	temp.dc = 0;
}

CanvasImpl::~CanvasImpl() {
    restoreObjects();
}

HDC CanvasImpl::getHDC() const {
    return dc;
}

void CanvasImpl::restoreObjects() {
	if( dc == 0 ) return;

    if( hpenOrig != 0 ) {
        HPEN hpen = (HPEN)SelectObject(dc,hpenOrig);
        DeleteObject(hpen);
        hpenOrig = 0;
    }
    if( hbrushOrig != 0 ) {
        HBRUSH hbrush = (HBRUSH) SelectObject(dc,hbrushOrig);
        DeleteObject(hbrush);
        hbrushOrig = 0;
    }
}

// TODO use stock objects and color instead of creating new pens and brushes
// see https://docs.microsoft.com/en-us/windows/desktop/gdi/setting-the-pen-or-brush-color

void CanvasImpl::setPen(Pen const & pen) {
	HPEN hpen = CreatePen(convertPenStyle(pen.style),pen.width,colorWin(pen.color));
	HPEN hpenold = (HPEN) SelectObject(dc,hpen);
	if( hpenOrig == 0 ) {
	    hpenOrig = hpenold;
	} else {
        DeleteObject(hpenold);
	}
}

// TODO this function will be used in the future for operations than need a selected HBRUSH
void CanvasImpl::setBrush(Brush const & brush) {
	HBRUSH hbrush = CreateSolidBrush(colorWin(brush.color));
	HBRUSH hbrushold = (HBRUSH) SelectObject(dc,hbrush);
	if( hbrushOrig == 0 ) {
	    hbrushOrig = hbrushold;
	} else {
        DeleteObject(hbrushold);
	}
}

void CanvasImpl::addClipRect(Rect const & openrect) {
	IntersectClipRect(dc,openrect.left,openrect.top,openrect.right,openrect.bottom);
}

void CanvasImpl::clearClipping() {
	SelectClipRgn(dc,NULL);
}

void CanvasImpl::translate(Point p) {
	SetViewportOrgEx(dc,p.x,p.y,0);
}

void CanvasImpl::clearTranslate() {
	SetViewportOrgEx(dc,0,0,0);
}



void CanvasImpl::plot(Point p1, RGBColor const & color) {
	SetPixel(dc,p1.x,p1.y,colorWin(color));
}

void CanvasImpl::drawLine(Point p1, Point p2, Pen const & pen) {
    setPen(pen);
	MoveToEx(dc,p1.x,p1.y,NULL);
	LineTo(dc,p2.x,p2.y);
}

void CanvasImpl::rectangle(Rect const & rect, Pen const & pen) {
    setPen(pen);
	MoveToEx(dc,rect.left,rect.top,NULL);
	LineTo(dc,rect.right,rect.top);
	LineTo(dc,rect.right,rect.bottom);
	LineTo(dc,rect.left,rect.bottom);
	LineTo(dc,rect.left,rect.top);
}

void CanvasImpl::fillRect(Rect const & rect, Brush const & brush) {
    auto cv = convertOpenRect(rect);
    scoped::Brush hbrush = CreateSolidBrush(colorWin(brush.color));
    FillRect(dc,&cv,hbrush.get());
}
//     HBRUSH hbrush = reinterpret_cast<HBRUSH>(GetCurrentObject(dc,OBJ_BRUSH));

void CanvasImpl::roundRect(Rect const & openrect, WDims ellipseDims, Pen const & pen, Brush const & brush) {
	setPen(pen);
	setBrush(brush);
	RoundRect(dc,openrect.left,openrect.top,openrect.right,openrect.bottom,ellipseDims.width,ellipseDims.height);
}

POINT makeWindowsPointFromGuiPoint(Point point)
{
	POINT result = {point.x,point.y};
	return result;
}

void CanvasImpl::drawPoly(Points const & polygon, Pen const & pen)
{
    setPen(pen);
	std::vector<POINT> points(polygon.size());
	std::transform(polygon.begin(),polygon.end(),points.begin(),&makeWindowsPointFromGuiPoint);
	Polyline(dc,&points[0],points.size());
}

void CanvasImpl::drawText(Point p, NativeString const & text, RGBColor const & color)
{
	SetTextColor(dc,colorWin(color));
	SetBkMode(dc,TRANSPARENT);
	TextOutW(dc,p.x,p.y,text.wstr.c_str(),text.wstr.size());
}

void CanvasImpl::drawText(Point p, NativeString const & text, RGBColor const & textColor, RGBColor const & backgroundColor) {
	SetTextColor(dc,colorWin(textColor));
	SetBkColor(dc,colorWin(backgroundColor));
	SetBkMode(dc,OPAQUE);
	TextOutW(dc,p.x,p.y,text.wstr.c_str(),text.wstr.size());
}

void CanvasImpl::textRect(Rect const & rect, NativeString const & text, TextParams const & params) {
	RECT winRect = convertRect(rect);
	SetTextColor(dc,colorWin(params.textColor));
	if( params.backgroundColor.has_value() ) {
		SetBkMode(dc,OPAQUE);
		SetBkColor(dc,colorWin(*params.backgroundColor));
	} else {
		SetBkMode(dc,TRANSPARENT);
	}
	DrawTextW(dc,text.wstr.c_str(),text.wstr.size(),&winRect,convertTextAlign(params.h_align,params.v_align));
}

WDims CanvasImpl::measureText(NativeString const & text)
{
    SIZE size;
    GetTextExtentPoint32W(dc,text.wstr.c_str(),text.wstr.size(),&size);
    return sizeToWDims(size);
}

void CanvasImpl::drawImage(image::Ptr const & image, tk::Point pos) {
	if( image == image::nullImage ) return;
	dynamic_cast<image::ImageImpl &>(*image).drawInto(*this,pos);
}

void CanvasImpl::drawImage(image::Ptr const & image, Rect destrect) {
	if( image == image::nullImage ) return;
	dynamic_cast<image::ImageImpl &>(*image).drawInto(*this,destrect);
}

void CanvasImpl::copyRectImage(tk::image::Ptr const & image, Rect srcrect, Point dest) {
	if( image == image::nullImage ) return;
	dynamic_cast<image::ImageImpl &>(*image).copyRectInto(*this,srcrect,dest);
}

// CanvasImplWin

CanvasImplWin::CanvasImplWin():
	hWnd(0)
{}

CanvasImplWin::CanvasImplWin(CanvasImplWin && temp):
    CanvasImpl(std::move(temp)),
	hWnd(temp.hWnd)
{
}

CanvasImplWin::~CanvasImplWin()
{
	releaseDC();
}

void CanvasImplWin::getDC(HWND hWnd)
{
	if( dc != 0 && this->hWnd == hWnd ) return;	   // already got a dc for this hWnd

	releaseDC();
	this->hWnd = hWnd;
	dc = GetDC(hWnd);
}

void CanvasImplWin::releaseDC()
{
	if( dc == 0 ) return;

	restoreObjects();

	ReleaseDC(hWnd,dc);
	dc = 0;
}

}
