#include "ImageWin32.h"
#include <sstream>
#include "../ResourceManager.h"
#include "../ConvertersWin32.h"
#include "../../components/Image.h"
#include "../../components/NullImage.h"
#include "../../log.h"
#include "../../NullCanvas.h"

namespace tk::image {

bool AlphaBlendTonT(HDC dcDest, int x, int y, int cx, int cy,
					HDC dcSrc, int sx, int sy, int scx, int scy);

BITMAPINFO createBitmapInfo(int width, int height, int bpp) {
	BITMAPINFO info;
	ZeroMemory(&info,sizeof(BITMAPINFO));
	BITMAPINFOHEADER & bHead = info.bmiHeader;
	bHead.biSize = sizeof(BITMAPINFOHEADER);
	bHead.biWidth = width;
	bHead.biHeight = height;
	bHead.biBitCount = bpp;
	bHead.biPlanes = 1;
	bHead.biCompression = BI_RGB;
	return info;
}

HBITMAP createBitmap(HDC dc, BITMAPINFO * info, Byte const * buffer) {
	HBITMAP result = CreateDIBitmap(dc,&info->bmiHeader,buffer == 0 ? 0 : CBM_INIT,buffer,info,DIB_RGB_COLORS);
	if( result == 0 ) {
		log::error("error creating bitmap for dc ",dc," and info header ",info->bmiHeader);
	}
	return result;
}

HBITMAP cloneBitmap(HDC dc, HBITMAP hbmp) {
	BITMAPINFO bInfo = {};
	bInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);	
	if( GetDIBits(dc,hbmp,0,0,0,&bInfo,DIB_RGB_COLORS) == 0 ) {
		log::error("GetDIBits failure while getting bmp info for cloning image");
		return 0;
	}
	bInfo.bmiHeader.biCompression = BI_RGB; // overriding bad result from GetDIBits
	
	HBITMAP result;
	HANDLE hDIB = GlobalAlloc(GMEM_MOVEABLE,bInfo.bmiHeader.biSizeImage); 
	if( hDIB == NULL ) {
		log::error("GlobalAlloc failed");
	}
	Byte * lpbitmap = (Byte *)GlobalLock(hDIB);	
	if( lpbitmap == NULL ) {
		log::error("GlobalLock failed");
	}
	if( GetDIBits(dc,hbmp,0,bInfo.bmiHeader.biHeight,lpbitmap,&bInfo,DIB_RGB_COLORS) == 0 ) {
		log::error("GetDIBits failure while getting bits for cloning image");
		result = 0;
	} else {
		result = createBitmap(dc,&bInfo,lpbitmap);
	}
	GlobalUnlock(hDIB);	
	GlobalFree(hDIB);
	return result;
}

WDims alignDims(WDims dims) {
	if( dims.width % 4 != 0 ) {
		auto remainder = 4 - dims.width % 4;
		dims.width += remainder;
	}
	return dims;
}

std::vector<Byte> rgbaToBgra(WDims dims, Byte const * buffer) {
	WDims normDims = alignDims(dims);
	std::vector<Byte> alignBuf(normDims.area()*4);
	for( auto h = 0; h != normDims.height; ++h ) {
		int s = h*dims.width*4;
		int d = h*normDims.width*4;
		for( auto w = 0; w != dims.width; ++w,s+=4,d+=4 ) {
			alignBuf[d] = buffer[s+2];
			alignBuf[d+1] = buffer[s+1];
			alignBuf[d+2] = buffer[s];
			alignBuf[d+3] = buffer[s+3];
		}
	}

	return alignBuf;
}

std::vector<Byte> rgbToBgr(WDims dims, Byte const * buffer) {
	WDims normDims = alignDims(dims);
	std::vector<Byte> alignBuf(normDims.area()*3);
	for( auto h = 0; h != normDims.height; ++h ) {
		int s = h*dims.width*3;
		int d = h*normDims.width*3;
		for( auto w = 0; w != dims.width; ++w,s+=3,d+=3 ) {
			alignBuf[d] = buffer[s+2];
			alignBuf[d+1] = buffer[s+1];
			alignBuf[d+2] = buffer[s];
		}
	}
	
	return alignBuf;
}

std::vector<Byte> grayToBgr(WDims dims, Byte const * buffer) {
	WDims normDims = alignDims(dims);
	std::vector<Byte> alignBuf(normDims.area()*3);
	for( auto h = 0; h != normDims.height; ++h ) {
		int s = h*dims.width;
		int d = h*normDims.width*3;
		for( auto w = 0; w != dims.width; ++w,s+=1,d+=3 ) {
			alignBuf[d] = buffer[s];
			alignBuf[d+1] = buffer[s];
			alignBuf[d+2] = buffer[s];
		}
	}
	
	return alignBuf;
}

// create //

std::shared_ptr<Image> create(Params const & params) {
	if( params.tryTransparent_ && params.bpp() == 32 ) {
		auto info = createBitmapInfo(params.dimensions_.width,params.dimensions_.height,32);
		if( params.buffer_ == 0 ) {
			return std::make_shared<BitmapAlpha>(&info,nullptr);
		}
		switch(params.type_) {
		case Type::BGRA:
			return std::make_shared<BitmapAlpha>(&info,params.buffer_);
		case Type::RGBA: {
			info.bmiHeader.biHeight = -info.bmiHeader.biHeight;
			auto swizzleBuf = rgbaToBgra(params.dimensions_,params.buffer_);
			return std::make_shared<BitmapAlpha>(&info,&swizzleBuf[0]);
		} break;
		}
	} else {	
		auto info = createBitmapInfo(params.dimensions_.width,params.dimensions_.height,24);
		if( params.buffer_ == 0 ) {
			return std::make_shared<Bitmap>(&info,nullptr);
		}

		switch(params.type_) {
			case Type::BGRA:
				info.bmiHeader.biBitCount = 32; // fall through
			case Type::BGR:
				return std::make_shared<Bitmap>(&info,params.buffer_);
			case Type::RGBA: {
				info.bmiHeader.biHeight = -info.bmiHeader.biHeight;
				auto swizzleBuf = rgbaToBgra(params.dimensions_,params.buffer_);
				return std::make_shared<Bitmap>(&info,&swizzleBuf[0]);
			} break;
			case Type::RGB: {
				info.bmiHeader.biHeight = -info.bmiHeader.biHeight;
				auto swizzleBuf = rgbToBgr(params.dimensions_,params.buffer_);
				return std::make_shared<Bitmap>(&info,&swizzleBuf[0]);
			} break;
			case Type::gray: {
				info.bmiHeader.biHeight = -info.bmiHeader.biHeight;
				auto swizzleBuf = grayToBgr(params.dimensions_,params.buffer_);
				return std::make_shared<Bitmap>(&info,&swizzleBuf[0]);
			} break;
		}
	}
	
	return nullImage;
}

// External buffer

External::External(BITMAPINFO * info, Byte const * buffer):
	info(info),buffer(buffer)
{}

External::External(Byte const * buffer):
	buffer(buffer)
{}

bool External::isBitmap() const {
	return false;
}

HBITMAP External::getOrCreateHbitmap() const {
	return cloneHbitmap();
}

HBITMAP External::cloneHbitmap() const {
	ResourceManagerSingleton resourceManager;
	return createBitmap(resourceManager->screenDC(),info,buffer);
}

void External::releaseIfCreated(HBITMAP hbmp) {
	DeleteObject(hbmp);
}

unsigned External::bpp() const {
	return info->bmiHeader.biBitCount;
}

WDims External::dims() const {
	auto & bHead = info->bmiHeader;
	return WDims(bHead.biWidth,abs(bHead.biHeight));
}

Canvas & External::beginDraw() {
	return nullCanvas; 
}

Canvas & External::canvas() {
	return nullCanvas; 
}

void External::endDraw()
{
}

void External::drawInto(CanvasImpl & canvas, Point dest) {
	auto d = dims();
	SetDIBitsToDevice(canvas.getHDC(),dest.x,dest.y,d.width,d.height,
		0,0,0,info->bmiHeader.biHeight,buffer,info,DIB_RGB_COLORS);
}

void External::drawInto(CanvasImpl & canvas, Rect destrect) {
	auto d = dims();
	auto destDims = destrect.dims();
	StretchDIBits(canvas.getHDC(),destrect.left,destrect.top,destDims.width,destDims.height,
		0,0,d.width,d.height,buffer,info,DIB_RGB_COLORS,SRCCOPY);
}

void External::copyRectInto(CanvasImpl & canvas, Rect srcrect, Point dest) {
	auto srcDims = srcrect.dims();
	StretchDIBits(canvas.getHDC(),dest.x,dest.y,srcDims.width,srcDims.height,
		srcrect.left,srcrect.top,srcDims.width,srcDims.height,buffer,info,DIB_RGB_COLORS,SRCCOPY);
}

// ExternalWithHeader

ExternalWithHeader::ExternalWithHeader(BITMAPINFO const & info_, Byte const * buffer):
	External(buffer),
	bitmapInfo(info_)
{
	info = &bitmapInfo;
}

// Bitmap 

Bitmap::Bitmap(BITMAPINFO * info, Byte const * buffer):
	Bitmap(createBitmap(ResourceManagerSingleton().instance().screenDC(),info,buffer))
{}

Bitmap::Bitmap(HBITMAP hbmp):
	bd(ResourceManagerSingleton().instance().screenDC(),hbmp),
	canvasImpl(bd.hdc)
{}

bool Bitmap::isBitmap() const {
	return true;
}

HBITMAP Bitmap::getOrCreateHbitmap() const {
	return getHbitmap();
}

HBITMAP Bitmap::getHbitmap() const {
	return bd.hbmp;
}

void Bitmap::releaseIfCreated(HBITMAP hbmp)
{
}

BITMAP Bitmap::getBitmap() const {
	BITMAP bitmap;
	GetObject(getHbitmap(),sizeof(BITMAP),&bitmap);
	return bitmap;	
}

HBITMAP Bitmap::cloneHbitmap() const {
	return cloneBitmap(ResourceManagerSingleton().instance().screenDC(),getHbitmap());
}

unsigned Bitmap::bpp() const {
	BITMAP bitmap = getBitmap();
	return bitmap.bmBitsPixel;
}

WDims Bitmap::dims() const {
	BITMAP bitmap = getBitmap();
	return WDims(bitmap.bmWidth,bitmap.bmHeight);
}

CanvasImpl & Bitmap::beginDraw() {
	bd.select();
	return canvas();
}

CanvasImpl & Bitmap::canvas() {
	return canvasImpl;
}

void Bitmap::endDraw() {
	bd.unselect();
}

void Bitmap::drawInto(CanvasImpl & canvas, Point dest) {
	auto d = dims();
	bd.select();
	if( BitBlt(canvas.getHDC(),dest.x,dest.y,d.width,d.height,bd.hdc,0,0,SRCCOPY) == 0 ) {
		log::error("BitBlt failed: ",dest,d);
	}
	bd.unselect();
}

void Bitmap::drawInto(CanvasImpl & canvas, Rect destrect) {
	auto d = dims();
	auto destDims = destrect.dims();
	bd.select();
	if( StretchBlt(canvas.getHDC(),destrect.left,destrect.top,destDims.width,destDims.height,bd.hdc,0,0,d.width,d.height,SRCCOPY) == 0 ) {
		log::error("StretchBlt failed: ",destrect,d);
	}
	bd.unselect();
}

void Bitmap::copyRectInto(CanvasImpl & canvas, Rect srcrect, Point dest) {
	auto srcDims = srcrect.dims();
	bd.select();
	StretchBlt(canvas.getHDC(),dest.x,dest.y,srcDims.width,srcDims.height,bd.hdc,srcrect.left,srcrect.top,srcDims.width,srcDims.height,SRCCOPY);
	bd.unselect();
}

// https://parnassus.co/transparent-graphics-with-pure-gdi-part-2-and-introducing-the-ttransparentcanvas-class/
// TODO fix the readability of this function
void Bitmap::replaceAllQuads(std::function<void(RGBQUAD &)> replacer) {
	auto cx = dims().width;
	auto cy = dims().height;
	bd.select();
	BITMAPINFOHEADER BMI = {};
	// Fill in the header info.
	BMI.biSize = sizeof(BITMAPINFOHEADER);
	BMI.biWidth = cx;
	BMI.biHeight = cy;
	BMI.biPlanes = 1;
	BMI.biBitCount = 32;
	BMI.biCompression = BI_RGB;   // No compression

	// Create DIB section in shared memory
	BYTE * pSrcBits;
	HBITMAP hbmSrc = CreateDIBSection(bd.hdc, (BITMAPINFO *) &BMI, DIB_RGB_COLORS, (void **) &pSrcBits, 0, 0l);
	// copy the bitmap pixels to the DIB
	HDC dc = CreateCompatibleDC(NULL);
	HBITMAP dcOld = (HBITMAP) SelectObject(dc, hbmSrc);
	BitBlt(dc,0,0,cx,cy,bd.hdc,0,0,SRCCOPY);
	SelectObject(dc, dcOld);
	DeleteDC(dc);

	// iterate over all DIB pixels
	for( int j = 0; j < cy; ++j ) {
		LPBYTE pbSrcRGBA = (LPBYTE) &((DWORD *) pSrcBits)[j * cx];
		for( int i = 0; i < cx; ++i ) {
			auto * q = reinterpret_cast<RGBQUAD *>(pbSrcRGBA);
			replacer(*q);
			pbSrcRGBA += 4;
		}
	}
//	auto * p = reinterpret_cast<unsigned char *>(bitmap.bmBits);
//	for( auto r = 0; r < bitmap.bmHeight; ++r ) {
//		auto * q = reinterpret_cast<RGBQUAD *>(p + r*bitmap.bmWidthBytes);
//		for( auto c = 0; c < bitmap.bmWidth; ++c ) {
//			replacer(q[c]);
//		}
//	}

	// copy the DIB pixels to the bitmap
	HDC dc2 = CreateCompatibleDC(NULL);
	SelectObject(dc2, hbmSrc);
	BitBlt(bd.hdc, 0, 0, cx, cy, dc2, 0, 0, SRCCOPY);
	DeleteDC(dc2);

	DeleteObject(hbmSrc);

	bd.unselect();
}

// BitmapAlpha

BitmapAlpha::BitmapAlpha(BITMAPINFO * info, Byte const * buffer):
	Bitmap(info,buffer)
{
	// Premultiply the image pixels with the alpha channel. Thanks to mysoft64bits@gmail.com who pointed this out.
	// NOTE: need to make sure this does not cause blending problems when drawing on top of other transparent canvases
	replaceAllQuads([](RGBQUAD & quad) {
		quad.rgbBlue = quad.rgbBlue * quad.rgbReserved / 255;
		quad.rgbGreen = quad.rgbGreen * quad.rgbReserved / 255;
		quad.rgbRed = quad.rgbRed * quad.rgbReserved / 255;
	});
}

void BitmapAlpha::drawInto(CanvasImpl & canvas, Point dest) {
	drawInto(canvas,Rect::closed(dest,dims()));
}

void BitmapAlpha::drawInto(CanvasImpl & canvas, Rect destrect) {
	alphaBlend(canvas,Rect::closed(Point(0,0),dims()),destrect);
}

void BitmapAlpha::copyRectInto(CanvasImpl & canvas, Rect srcrect, Point dest) {
	srcrect = srcrect.fitInRect(Rect::closed(Point(),dims()));
	alphaBlend(canvas,srcrect,srcrect.move(dest));
}

void BitmapAlpha::alphaBlend(CanvasImpl & canvas, Rect srcrect, Rect destrect) {
	auto srcDims = srcrect.dims();
	auto destDims = destrect.dims();

	bd.select();
	BLENDFUNCTION blendFunction;
	blendFunction.BlendOp = AC_SRC_OVER;
	blendFunction.BlendFlags = 0;
	blendFunction.SourceConstantAlpha = 0xFF;
	blendFunction.AlphaFormat = AC_SRC_ALPHA;
	if( GdiAlphaBlend(canvas.getHDC(),destrect.left,destrect.top,destDims.width,destDims.height,
		bd.hdc,srcrect.left,srcrect.top,srcDims.width,srcDims.height,
		blendFunction) == 0 ) {
		log::error("GdiAlphaBlend failed: ",srcrect,destrect);
	}
	bd.unselect();
}

void BitmapAlpha::drawTonT(CanvasImpl & canvas, Point dest) {
	alphaBlendTonT(canvas, Rect::closed(Point(0, 0), dims()), Rect::closed(dest, dims()));
}

void BitmapAlpha::alphaBlendTonT(CanvasImpl & canvas, Rect srcrect, Rect destrect) {
//	log::info("drawing T at ",srcrect," on T ",destrect);
	auto srcDims = srcrect.dims();
	auto destDims = destrect.dims();

	bd.select();
	if( !AlphaBlendTonT(canvas.getHDC(), destrect.left, destrect.top, destDims.width, destDims.height,
						bd.hdc, srcrect.left, srcrect.top, srcDims.width, srcDims.height) ) {
		log::error("AlphaBlendTonT failed: ",srcrect,destrect);
	}
	bd.unselect();
}

// BitmapPallete

COLORREF transpColor = RGB(254,253,255);

BITMAPINFO * hackTransp(BITMAPINFO * info, int transparentIndex) {
	// HACK: get the color entry in the color table and change to some color that *may* not be being used
	auto & transparentColor = info->bmiColors[transparentIndex];
	transparentColor.rgbRed = 254;
	transparentColor.rgbGreen = 253;
	transparentColor.rgbBlue = 255;
	return info;
}

BitmapPallete::BitmapPallete(BITMAPINFO * info, Byte const * buffer, int transparentIndex):
	Bitmap(hackTransp(info,transparentIndex),buffer)
{
}

void BitmapPallete::drawInto(CanvasImpl & canvas, Point dest) {
	drawInto(canvas,Rect::closed(dest,dims()));
}

void BitmapPallete::drawInto(CanvasImpl & canvas, Rect destrect) {
	drawInto(canvas,Rect::closed(Point(0,0),dims()),destrect);
}

void BitmapPallete::copyRectInto(CanvasImpl & canvas, Rect srcrect, Point dest) {
	drawInto(canvas,srcrect,srcrect.move(dest));
}

void BitmapPallete::drawInto(CanvasImpl & canvas, Rect srcrect, Rect destrect) {
	auto srcDims = srcrect.dims();
	auto destDims = destrect.dims();
	
	bd.select();
	::GdiTransparentBlt(canvas.getHDC(),destrect.left,destrect.top,destDims.width,destDims.height,
		bd.hdc,srcrect.left,srcrect.top,srcDims.width,srcDims.height,transpColor);
	bd.unselect();
}

// adapted from http://www.drdobbs.com/transparentblt-and-alphablend-for-window/184416353?pgno=11
bool AlphaBlendTonT(HDC dcDest, int x, int y, int cx, int cy,
					HDC dcSrc, int sx, int sy, int scx, int scy) {
	BITMAPINFOHEADER BMI;
	// Fill in the header info.
	BMI.biSize = sizeof(BITMAPINFOHEADER);
	BMI.biWidth = cx;
	BMI.biHeight = cy;
	BMI.biPlanes = 1;
	BMI.biBitCount = 32;
	BMI.biCompression = BI_RGB;   // No compression
	BMI.biSizeImage = 0;
	BMI.biXPelsPerMeter = 0;
	BMI.biYPelsPerMeter = 0;
	BMI.biClrUsed = 0;           // Always use the whole palette.
	BMI.biClrImportant = 0;

	BYTE * pSrcBits;
	HBITMAP hbmSrc;
	// Create DIB section in shared memory
	hbmSrc = CreateDIBSection(dcSrc, (BITMAPINFO *) &BMI, DIB_RGB_COLORS, (void **) &pSrcBits, 0, 0l);

	BYTE * pDestBits;
	HBITMAP hbmDest;
	// Create DIB section in shared memory
	hbmDest = CreateDIBSection(dcDest, (BITMAPINFO *) &BMI, DIB_RGB_COLORS, (void **) &pDestBits, 0, 0l);

	// Copy our source and destination bitmaps onto our DIBSections,
	// so we can get access to their bits using the BYTE *'s we
	// passed into CreateDIBSection
	HDC dc = CreateCompatibleDC(NULL);
	HBITMAP dcOld = (HBITMAP) SelectObject(dc, hbmSrc);
	if( !StretchBlt(dc, 0, 0, cx, cy, dcSrc, sx, sy, scx, scy, SRCCOPY)) return false;
	SelectObject(dc, hbmDest);
	if( !StretchBlt(dc, 0, 0, cx, cy, dcDest, x, y, cx, cy, SRCCOPY)) return false;
	SelectObject(dc, dcOld);
	DeleteDC(dc);

	auto blend = [](BYTE src, BYTE dst, BYTE a) -> BYTE {
		if( a == 255 ) return src;
		if( a == 0 ) return dst;
		return (src * a + dst * (255-a)) / 255;
	};

	for( int j = 0; j < cy; ++j ) {
		LPBYTE pbDestRGB = (LPBYTE) &((DWORD *) pDestBits)[j * cx];
		LPBYTE pbSrcRGB = (LPBYTE) &((DWORD *) pSrcBits)[j * cx];
		for( int i = 0; i < cx; ++i ) {
			pbSrcRGB[0] = blend(pbSrcRGB[0],pbDestRGB[0],pbSrcRGB[3]);
			pbSrcRGB[1] = blend(pbSrcRGB[1],pbDestRGB[1],pbSrcRGB[3]);
			pbSrcRGB[2] = blend(pbSrcRGB[2],pbDestRGB[2],pbSrcRGB[3]);
//				if( (pbSrcRGB[3] != 0 && pbSrcRGB[3] != 255) || (pbDestRGB[3] != 0 && pbDestRGB[3] != 255) ) {
//					log::info(cx,',',cy," s=",pbSrcRGB[3]," t=",pbDestRGB[3]);
//				}
			// when src = transp and dst = untouched, keep the transparency
			if( pbSrcRGB[3] == 0 && pbDestRGB[3] == 255 ) {
				pbSrcRGB[3] = 255;  // will be 0 afterwards
			} else {
				// all other cases, make the pixel opaque - this can't be reverted
				pbSrcRGB[3] = 0;    // will be 255 afterwards
			}
			// pbSrcRGB[3] = blend(pbSrcRGB[3],pbDestRGB[3],pbSrcRGB[3]);
			pbSrcRGB += 4;
			pbDestRGB += 4;
		}
	}

	dc = CreateCompatibleDC(NULL);

	SelectObject(dc, hbmSrc);
	if( !BitBlt(dcDest, x, y, cx, cy, dc, 0, 0, SRCCOPY) ) return false;
	DeleteDC(dc);

	DeleteObject(hbmSrc);
	DeleteObject(hbmDest);

	return true;
}

std::string info(Ptr const & image) {
	auto ss = std::ostringstream();
	ss << "dims " << image->dims() << " bpp " << image->bpp() << " type " << typeid(*image).name();
	return ss.str();
}

} // namespace tk::image

std::ostream & operator<<(std::ostream & os, BITMAPINFOHEADER const & bh) {
	os 
		<< "biWidth " << bh.biWidth
		<< " biHeight " << bh.biHeight
		<< " biBitCount " << bh.biBitCount
		<< " biPlanes " << bh.biPlanes
		<< " biSize " << bh.biSize
		<< " biSizeImage " << bh.biSizeImage
		<< " biCompression " << bh.biCompression
		<< " biClrImportant " << bh.biClrImportant
		<< " biClrUsed " << bh.biClrUsed
		<< " biXPelsPerMeter " << bh.biXPelsPerMeter
		<< " biYPelsPerMeter " << bh.biYPelsPerMeter
		;
	return os;
}
