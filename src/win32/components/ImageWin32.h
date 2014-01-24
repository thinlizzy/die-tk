#ifndef IMAGE_WIN32_H_DIE_TK_2014_01_19_11_01
#define IMAGE_WIN32_H_DIE_TK_2014_01_19_11_01

#include "../../components/Image.h"
#include <windows.h>
#include "../ScopedObjects.h"

namespace tk {
    
namespace image {

class ImageImpl: public Image {
public:
    virtual bool isBitmap() const = 0;
    
    virtual HBITMAP getOrCreateHbitmap() const = 0;
    virtual void releaseIfCreated(HBITMAP hbmp) = 0;
    
    virtual HBITMAP cloneHbitmap() const = 0;
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
    virtual void drawInto(Canvas & canvas, Point dest);
    virtual void drawInto(Canvas & canvas, Rect destrect);
};

class ExternalWithHeader: public External {
    BITMAPINFO bitmapInfo;
public:
    ExternalWithHeader(BITMAPINFO const & info, Byte const * buffer);
};

class Bitmap: public ImageImpl {
protected:
    tk::scoped::BitmapDC bd;
public:
    Bitmap(BITMAPINFO * info, Byte const * buffer);
    explicit Bitmap(HBITMAP hbmp);

    virtual bool isBitmap() const;
    virtual HBITMAP getOrCreateHbitmap() const;
    virtual void releaseIfCreated(HBITMAP hbmp);
    virtual HBITMAP cloneHbitmap() const;

    virtual unsigned bpp() const;
    virtual WDims dims() const;
    virtual void drawInto(Canvas & canvas, Point dest);
    virtual void drawInto(Canvas & canvas, Rect destrect);
    
    HBITMAP getHbitmap() const;
private:
    BITMAP getBitmap() const;
};

class BitmapAlpha: public Bitmap {
public:    
    BitmapAlpha(BITMAPINFO * info, Byte const * buffer);
    
    virtual void drawInto(Canvas & canvas, Point dest);
    virtual void drawInto(Canvas & canvas, Rect destrect);
};

class BitmapPallete: public Bitmap {
public:    
    BitmapPallete(BITMAPINFO * info, Byte const * buffer, int transparentIndex);
    
    virtual void drawInto(Canvas & canvas, Point dest);
    virtual void drawInto(Canvas & canvas, Rect destrect);
};

}

}

#endif