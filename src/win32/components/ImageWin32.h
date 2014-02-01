#ifndef IMAGE_WIN32_H_DIE_TK_2014_01_19_11_01
#define IMAGE_WIN32_H_DIE_TK_2014_01_19_11_01

#include "../../components/Image.h"
#include "../ScopedObjects.h"
#include "../CanvasImplWin32.h"
#include <ostream>
#include <windows.h>

namespace tk {
    
namespace image {

class ImageImpl: public Image {
public:
    virtual void drawInto(HDC dc, Point dest) = 0;
    virtual void drawInto(HDC dc, Rect destrect) = 0;
    
    virtual bool isBitmap() const = 0;
    
    virtual HBITMAP getOrCreateHbitmap() const = 0;
    virtual void releaseIfCreated(HBITMAP hbmp) = 0;
    
    virtual HBITMAP cloneHbitmap() const = 0;
};

class Null: public ImageImpl {
public:
    void drawInto(HDC dc, Point dest) override {}
    void drawInto(HDC dc, Rect destrect) override {}
    bool isBitmap() const override { return false; }
    HBITMAP getOrCreateHbitmap() const override { return 0; }
    void releaseIfCreated(HBITMAP hbmp) override {}
    HBITMAP cloneHbitmap() const override { return 0; }
    
    unsigned bpp() const override { return 0; }
    WDims dims() const override { return WDims(); }
    Canvas & beginDraw() override { return canvas(); }
    Canvas & canvas() override ;
    void endDraw() override {}
};

// handles images with externally owned buffers
class External: public ImageImpl {
protected:
    BITMAPINFO * info;
    External(Byte const * buffer);
private:
    Byte const * buffer;
public:
    External(BITMAPINFO * info, Byte const * buffer);
    
    virtual bool isBitmap() const;    
    virtual HBITMAP getOrCreateHbitmap() const;    
    virtual void releaseIfCreated(HBITMAP hbmp);
    virtual HBITMAP cloneHbitmap() const;

    virtual unsigned bpp() const;
    virtual WDims dims() const;

    virtual Canvas & beginDraw();
    virtual Canvas & canvas();
    virtual void endDraw();
    
    virtual void drawInto(HDC dc, Point dest);
    virtual void drawInto(HDC dc, Rect destrect);
};

class ExternalWithHeader: public External {
    BITMAPINFO bitmapInfo;
public:
    ExternalWithHeader(BITMAPINFO const & info, Byte const * buffer);
};

class Bitmap: public ImageImpl {
protected:
    tk::scoped::BitmapDC bd;
    tk::CanvasImpl canvasImpl;
public:
    Bitmap(BITMAPINFO * info, Byte const * buffer);
    explicit Bitmap(HBITMAP hbmp);

    virtual bool isBitmap() const;
    virtual HBITMAP getOrCreateHbitmap() const;
    virtual void releaseIfCreated(HBITMAP hbmp);
    virtual HBITMAP cloneHbitmap() const;

    virtual unsigned bpp() const;
    virtual WDims dims() const;

    virtual Canvas & beginDraw();
    virtual Canvas & canvas();
    virtual void endDraw();
    
    virtual void drawInto(HDC dc, Point dest);
    virtual void drawInto(HDC dc, Rect destrect);
    
    HBITMAP getHbitmap() const;
private:
    BITMAP getBitmap() const;
};

class BitmapAlpha: public Bitmap {
public:    
    BitmapAlpha(BITMAPINFO * info, Byte const * buffer);
    
    virtual void drawInto(HDC dc, Point dest);
    virtual void drawInto(HDC dc, Rect destrect);
};

class BitmapPallete: public Bitmap {
public:    
    BitmapPallete(BITMAPINFO * info, Byte const * buffer, int transparentIndex);
    
    virtual void drawInto(HDC dc, Point dest);
    virtual void drawInto(HDC dc, Rect destrect);
};

typedef std::shared_ptr<Bitmap> BitmapPtr;

inline BitmapPtr createBitmap(HBITMAP hbmp) { return std::make_shared<Bitmap>(hbmp); }
inline BitmapPtr cloneBitmap(std::shared_ptr<ImageImpl> image) { return createBitmap(image->cloneHbitmap()); }

HBITMAP cloneBitmap(HDC dc, HBITMAP hbmp);

}

}

std::ostream & operator<<(std::ostream & os, BITMAPINFOHEADER const & bh);

#endif