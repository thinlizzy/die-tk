#include "ImageWin32.h"
#include "../ResourceManager.h"
#include "../ConvertersWin32.h"
#include "../../components/Image.h"
#include "../../log.h"
#include "../../NullCanvas.h"
#include "../../components/NullImage.h"

namespace tk {
    
namespace image {
    
BITMAPINFO createBitmapInfo(int width, int height, int bpp)
{
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

HBITMAP createBitmap(HDC dc, BITMAPINFO * info, Byte const * buffer)
{
    HBITMAP result = CreateDIBitmap(dc,&info->bmiHeader,buffer == 0 ? 0 : CBM_INIT,buffer,info,DIB_RGB_COLORS);
    if( result == 0 ) {
        log::error("error creating bitmap for dc ",dc," and info header ",info->bmiHeader);
    }
    return result;
}

HBITMAP cloneBitmap(HDC dc, HBITMAP hbmp)
{
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

WDims alignDims(WDims dims)
{
	if( dims.width % 4 != 0 ) {
		auto remainder = 4 - dims.width % 4;
		dims.width += remainder;
    }
    return dims;
}

std::vector<Byte> rgbaToBgra(WDims dims, Byte const * buffer)
{
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

std::vector<Byte> rgbToBgr(WDims dims, Byte const * buffer)
{
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

std::vector<Byte> grayToBgr(WDims dims, Byte const * buffer)
{
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

std::shared_ptr<Image> create(Params const & params)
{
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

HDC hdc(Canvas & canvas) {
	if( &canvas == &nullCanvas ) return 0;

	auto & canvasWin = static_cast<CanvasImpl &>(canvas);
	return canvasWin.getHDC();
}

// External buffer

External::External(BITMAPINFO * info, Byte const * buffer):
    info(info),buffer(buffer)
{}

External::External(Byte const * buffer):
    buffer(buffer)
{}

bool External::isBitmap() const
{
    return false;
}

HBITMAP External::getOrCreateHbitmap() const
{
    return cloneHbitmap();
}

HBITMAP External::cloneHbitmap() const
{
    ResourceManagerSingleton resourceManager;
    return createBitmap(resourceManager->screenDC(),info,buffer);
}

void External::releaseIfCreated(HBITMAP hbmp)
{
    DeleteObject(hbmp);
}

unsigned External::bpp() const
{
    return info->bmiHeader.biBitCount;
}

WDims External::dims() const
{
	auto & bHead = info->bmiHeader;
    return WDims(bHead.biWidth,abs(bHead.biHeight));
}

Canvas & External::beginDraw() 
{ 
    return nullCanvas; 
}

Canvas & External::canvas() 
{ 
    return nullCanvas; 
}

void External::endDraw()
{
}

void External::drawInto(Canvas & canvas, Point dest)
{
	auto d = dims();
	SetDIBitsToDevice(hdc(canvas),dest.x,dest.y,d.width,d.height,
		0,0,0,info->bmiHeader.biHeight,buffer,info,DIB_RGB_COLORS);
}

void External::drawInto(Canvas & canvas, Rect destrect)
{
	auto d = dims();
	auto destDims = destrect.dims();
	StretchDIBits(hdc(canvas),destrect.left,destrect.top,destDims.width,destDims.height,
		0,0,d.width,d.height,buffer,info,DIB_RGB_COLORS,SRCCOPY);
}

void External::copyRectInto(Canvas & canvas, Rect srcrect, Point dest)
{
	auto srcDims = srcrect.dims();
	StretchDIBits(hdc(canvas),dest.x,dest.y,srcDims.width,srcDims.height,
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

bool Bitmap::isBitmap() const
{
    return true;
}

HBITMAP Bitmap::getOrCreateHbitmap() const
{
    return getHbitmap();
}

HBITMAP Bitmap::getHbitmap() const
{
    return bd.hbmp;
}

void Bitmap::releaseIfCreated(HBITMAP hbmp)
{
}

BITMAP Bitmap::getBitmap() const
{
    BITMAP bitmap;
    GetObject(getHbitmap(),sizeof(BITMAP),&bitmap);
    return bitmap;    
}

HBITMAP Bitmap::cloneHbitmap() const
{
    return cloneBitmap(ResourceManagerSingleton().instance().screenDC(),getHbitmap());
}

unsigned Bitmap::bpp() const
{
    BITMAP bitmap = getBitmap();
    return bitmap.bmBitsPixel;
}

WDims Bitmap::dims() const
{
    BITMAP bitmap = getBitmap();
    return WDims(bitmap.bmWidth,bitmap.bmHeight);
}

Canvas & Bitmap::beginDraw() 
{ 
    bd.select();
    return canvas();
}

Canvas & Bitmap::canvas() 
{ 
    return canvasImpl;
}

void Bitmap::endDraw()
{
    bd.unselect();
}

void Bitmap::drawInto(Canvas & canvas, Point dest)
{
    auto d = dims();
    bd.select();
    BitBlt(hdc(canvas),dest.x,dest.y,d.width,d.height,bd.hdc,0,0,SRCCOPY);
    bd.unselect();
}

void Bitmap::drawInto(Canvas & canvas, Rect destrect)
{
    auto d = dims();
    auto destDims = destrect.dims();
    bd.select();
    StretchBlt(hdc(canvas),destrect.left,destrect.top,destDims.width,destDims.height,
            bd.hdc,0,0,d.width,d.height,SRCCOPY);
    bd.unselect();
}

void Bitmap::copyRectInto(Canvas & canvas, Rect srcrect, Point dest)
{
	auto srcDims = srcrect.dims();
    bd.select();
    StretchBlt(hdc(canvas),dest.x,dest.y,srcDims.width,srcDims.height,
            bd.hdc,srcrect.left,srcrect.top,srcDims.width,srcDims.height,SRCCOPY);
    bd.unselect();
}

// BitmapAlpha

BitmapAlpha::BitmapAlpha(BITMAPINFO * info, Byte const * buffer):
    Bitmap(info,buffer)
{
}

void BitmapAlpha::drawInto(Canvas & canvas, Point dest)
{
    drawInto(canvas,Rect::closed(dest,dims()));
}

void BitmapAlpha::drawInto(Canvas & canvas, Rect destrect)
{
	drawInto(canvas,Rect::closed(Point(0,0),dims()),destrect);
}

void BitmapAlpha::copyRectInto(Canvas & canvas, Rect srcrect, Point dest)
{
    srcrect = srcrect.fitInRect(Rect::closed(Point(),dims()));
	drawInto(canvas,srcrect,srcrect.move(dest));
}

void BitmapAlpha::drawInto(Canvas & canvas, Rect srcrect, Rect destrect)
{
    auto srcDims = srcrect.dims();
    auto destDims = destrect.dims();

    BLENDFUNCTION bf;
    bf.BlendOp = AC_SRC_OVER;
    bf.BlendFlags = 0;
    bf.SourceConstantAlpha = 0xFF;
    bf.AlphaFormat = AC_SRC_ALPHA;
    bd.select();
    ::GdiAlphaBlend(hdc(canvas),destrect.left,destrect.top,destDims.width,destDims.height,
		bd.hdc,srcrect.left,srcrect.top,srcDims.width,srcDims.height,bf);
    bd.unselect();
}

// BitmapPallete

COLORREF transpColor = RGB(254,253,255);

BITMAPINFO * hackTransp(BITMAPINFO * info, int transparentIndex)
{
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

void BitmapPallete::drawInto(Canvas & canvas, Point dest)
{
    drawInto(canvas,Rect::closed(dest,dims()));
}

void BitmapPallete::drawInto(Canvas & canvas, Rect destrect)
{
	drawInto(canvas,Rect::closed(Point(0,0),dims()),destrect);
}

void BitmapPallete::copyRectInto(Canvas & canvas, Rect srcrect, Point dest)
{
	drawInto(canvas,srcrect,srcrect.move(dest));
}

void BitmapPallete::drawInto(Canvas & canvas, Rect srcrect, Rect destrect)
{
    auto srcDims = srcrect.dims();
    auto destDims = destrect.dims();
    
    bd.select();
    ::GdiTransparentBlt(hdc(canvas),destrect.left,destrect.top,destDims.width,destDims.height,
		bd.hdc,srcrect.left,srcrect.top,srcDims.width,srcDims.height,transpColor);
    bd.unselect();
}

} // namespace image

} // namespace tk

std::ostream & operator<<(std::ostream & os, BITMAPINFOHEADER const & bh)
{
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
