#include "CanvasImplWin32.h"
#include "ConvertersWin32.h"
#include <algorithm>
#include <stdexcept>

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

CanvasImpl::~CanvasImpl()
{
    restoreObjects();
}

void CanvasImpl::restoreObjects()
{
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


void CanvasImpl::setPen(Pen const & pen)
{
	HPEN hpen = CreatePen(pen.style,pen.width,colorWin(pen.color));
	HPEN hpenold = (HPEN) SelectObject(dc,hpen);
	if( hpenOrig == 0 ) {
	    hpenOrig = hpenold;
	} else {
        DeleteObject(hpenold);
	}
}

void CanvasImpl::setBrush(Brush const & brush)
{
	HBRUSH hbrush = CreateSolidBrush(colorWin(brush.color));
	HBRUSH hbrushold = (HBRUSH) SelectObject(dc,hbrush);
	if( hbrushOrig == 0 ) {
	    hbrushOrig = hbrushold;
	} else {
        DeleteObject(hbrushold);
	}
}

void CanvasImpl::addClipRect(Rect const & openrect)
{
	IntersectClipRect(dc,openrect.left,openrect.top,openrect.right,openrect.bottom);
}

void CanvasImpl::clearClipping()
{
	SelectClipRgn(dc,NULL);
}

void CanvasImpl::translate(Point p)
{
	SetViewportOrgEx(dc,p.x,p.y,0);
}

void CanvasImpl::clearTranslate()
{
	SetViewportOrgEx(dc,0,0,0);
}




void CanvasImpl::plot(Point p1, RGBColor const & color)
{
	SetPixel(dc,p1.x,p1.y,colorWin(color));
}

void CanvasImpl::drawLine(Point p1, Point p2)
{
	MoveToEx(dc,p1.x,p1.y,NULL);
	LineTo(dc,p2.x,p2.y);
}

void CanvasImpl::rectangle(Rect const & rect)
{
	MoveToEx(dc,rect.left,rect.top,NULL);
	LineTo(dc,rect.right,rect.top);
	LineTo(dc,rect.right,rect.bottom);
	LineTo(dc,rect.left,rect.bottom);
	LineTo(dc,rect.left,rect.top);
}

void CanvasImpl::fillRect(Rect const & rect)
{
	Rectangle(dc,rect.left,rect.top,rect.right,rect.bottom);
}

void CanvasImpl::fillRect(RECT rect)
{
	Rectangle(dc,rect.left,rect.top,rect.right,rect.bottom);
}


POINT makeWindowsPointFromGuiPoint(Point point)
{
	POINT result = {point.x,point.y};
	return result;
}

void CanvasImpl::drawPoly(Points const & polygon)
{
	std::vector<POINT> points(polygon.size());
	std::transform(polygon.begin(),polygon.end(),points.begin(),&makeWindowsPointFromGuiPoint);
	Polyline(dc,&points[0],points.size());
}


void CanvasImpl::drawImage(Point dest, unsigned char const buffer[], void * header)
{
	auto info = reinterpret_cast<BITMAPINFO *>(header);
	auto & bHead = info->bmiHeader;
	SetDIBitsToDevice(dc,dest.x,dest.y,bHead.biWidth,abs(bHead.biHeight),0,0,0,bHead.biHeight,buffer,info,DIB_RGB_COLORS);
}

void CanvasImpl::drawImage(Rect const & destrect, Point start, unsigned char const buffer[], void * header)
{
	auto info = reinterpret_cast<BITMAPINFO *>(header);
	auto & bHead = info->bmiHeader;
	SetDIBitsToDevice(dc,destrect.left,destrect.top,destrect.openDims().width,destrect.openDims().height,start.x,start.y,0,bHead.biHeight,buffer,info,DIB_RGB_COLORS);
}


void CanvasImpl::drawImage(Point start, WDims dim, unsigned char const buffer[], ImageType type)
{
	BITMAPINFO info;
	ZeroMemory(&info,sizeof(BITMAPINFO));
	BITMAPINFOHEADER & bHead = info.bmiHeader;
	bHead.biSize = sizeof(BITMAPINFOHEADER);
	bHead.biWidth = dim.width;
	bHead.biHeight = -dim.height;					// -height to invert the image!!!
	bHead.biPlanes = 1;
	bHead.biBitCount = 24;
	bHead.biCompression = BI_RGB;
	bHead.biSizeImage = 0;
	bHead.biXPelsPerMeter = 0;
	bHead.biYPelsPerMeter = 0;
	bHead.biClrUsed = 0;
	bHead.biClrImportant = 0;

	// ensure alignment (slow) -- The scan lines must be aligned on a DWORD except for RLE-compressed bitmaps.
	std::vector<unsigned char> alignBuf;
	auto normDim = dim;
	if( dim.width % 4 != 0 ) {
		auto remainder = 4 - dim.width % 4;
		normDim.width += remainder;
		auto bpp = type == it_gray ? 1 : 3;
		alignBuf.resize(normDim.area()*bpp);
		for( auto h = 0; h != dim.height; ++h ) {
			std::copy(&buffer[h*dim.width*bpp],&buffer[(h+1)*dim.width*bpp],&alignBuf[h*normDim.width*bpp]);
		}

		buffer = &alignBuf[0];
		bHead.biWidth = normDim.width;
	}

	void const * imageBits;
	std::vector<unsigned char> swizzleBuf;
	switch(type) {
		case it_BGR:			// native GDI format
			imageBits = buffer;
			break;
		case it_RGB:			// we have to swizzle :( slowwww www w w ww w
			swizzleBuf.resize(normDim.area()*3);
			for( unsigned p = 0; p != swizzleBuf.size(); p += 3 ) {
				swizzleBuf[p] = buffer[p+2];
				swizzleBuf[p+1] = buffer[p+1];
				swizzleBuf[p+2] = buffer[p];
			}
			imageBits = &swizzleBuf[0];
			break;
		case it_gray:			// we have to expand the component colors :( quite slow
			swizzleBuf.resize(normDim.area()*3);
			for( unsigned p = 0, b = 0; p != swizzleBuf.size(); p += 3, ++b ) {
				swizzleBuf[p] = buffer[b];
				swizzleBuf[p+1] = buffer[b];
				swizzleBuf[p+2] = buffer[b];
			}
			imageBits = &swizzleBuf[0];
			break;
		default:
			throw std::invalid_argument("unknown image type");
	}

	// which is better? :(
	SetDIBitsToDevice(dc,start.x,start.y,dim.width,abs(dim.height),0,0,0,dim.height,imageBits,&info,DIB_RGB_COLORS);
	// StretchDIBits(dc,start.x,start.y,dim.width,dim.height,0,0,dim.width,dim.height,imageBits,&info,DIB_RGB_COLORS,SRCCOPY);
}

void CanvasImpl::drawImage(Rect const & rect, unsigned char const buffer[], ImageType type)
{
	drawImage(Point(rect.left,rect.top),rect.openDims(),buffer,type);
}

static RECT convertRect(Rect const & rect)
{
	RECT result = { rect.left, rect.top, rect.right, rect.bottom };
	return result;
}

void CanvasImpl::textRect(Rect const & rect, std::string const & text, TextParams const & params)
{
	RECT winRect = convertRect(rect);
	// set colors
	SetTextColor(dc,colorWin(params.textColor));
	SetBkColor(dc,colorWin(params.backgroundColor));
	// set alignment
	UINT format = 0;
	switch( params.h_align ) {
		case hta_left:
			format |= DT_LEFT;
			break;
		case hta_right:
			format |= DT_RIGHT;
			break;
		case hta_center:
			format |= DT_CENTER;
			break;
	};
	switch( params.v_align ) {
		case vta_top:
			format |= DT_TOP;
			break;
		case vta_bottom:
			format |= DT_BOTTOM;
			format |= DT_SINGLELINE;
			break;
		case vta_center:
			format |= DT_VCENTER;
			format |= DT_SINGLELINE;
			break;
	};
	DrawText(dc,text.c_str(),text.size(),&winRect,format);
}


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
