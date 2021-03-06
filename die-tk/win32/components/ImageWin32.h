#ifndef IMAGE_WIN32_H_DIE_TK_2014_01_19_11_01
#define IMAGE_WIN32_H_DIE_TK_2014_01_19_11_01

#include "../../components/Image.h"
#include "../ScopedObjects.h"
#include "../CanvasImplWin32.h"
#include <functional>
#include <string>
#include <ostream>
#include <windows.h>

namespace tk::image {

class ImageImpl: public Image {
public:
	virtual bool isBitmap() const = 0;
	
	virtual HBITMAP getOrCreateHbitmap() const = 0;
	virtual void releaseIfCreated(HBITMAP hbmp) = 0;
	
	virtual HBITMAP cloneHbitmap() const = 0;

	virtual void drawInto(CanvasImpl & canvas, Point dest) = 0;
	virtual void drawInto(CanvasImpl & canvas, Rect destrect) = 0;
	virtual void copyRectInto(CanvasImpl & canvas, Rect srcrect, Point dest) = 0;
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
	
	bool isBitmap() const override;
	HBITMAP getOrCreateHbitmap() const override;
	void releaseIfCreated(HBITMAP hbmp) override;
	HBITMAP cloneHbitmap() const override;

	unsigned bpp() const override;
	WDims dims() const override;

	Canvas & beginDraw() override;
	Canvas & canvas() override;
	void endDraw() override;
	
	void drawInto(CanvasImpl & canvas, Point dest) override;
	void drawInto(CanvasImpl & canvas, Rect destrect) override;
	void copyRectInto(CanvasImpl & canvas, Rect srcrect, Point dest) override;
};

class ExternalWithHeader: public External {
	BITMAPINFO bitmapInfo;
public:
	ExternalWithHeader(BITMAPINFO const & info, Byte const * buffer);
};

class Bitmap: public ImageImpl {
protected:
	scoped::BitmapDC bd;
	CanvasImpl canvasImpl;
public:
	Bitmap(BITMAPINFO * info, Byte const * buffer);
	explicit Bitmap(HBITMAP hbmp);

	bool isBitmap() const override;
	HBITMAP getOrCreateHbitmap() const override;
	void releaseIfCreated(HBITMAP hbmp) override;
	HBITMAP cloneHbitmap() const override;

	unsigned bpp() const override;
	WDims dims() const override;

	CanvasImpl & beginDraw() override;
	CanvasImpl & canvas() override;
	void endDraw() override;

	void drawInto(CanvasImpl & canvas, Point dest) override;
	void drawInto(CanvasImpl & canvas, Rect destrect) override;
	void copyRectInto(CanvasImpl & canvas, Rect srcrect, Point dest) override;
	
	HBITMAP getHbitmap() const;
	void replaceAllQuads(std::function<void(RGBQUAD &)> replacer);
private:
	BITMAP getBitmap() const;
};

class BitmapAlpha: public Bitmap {
public:	
	BitmapAlpha(BITMAPINFO * info, Byte const * buffer);

	void drawInto(CanvasImpl & canvas, Point dest) override;
	void drawInto(CanvasImpl & canvas, Rect destrect) override;
	void copyRectInto(CanvasImpl & canvas, Rect srcrect, Point dest) override;

	void drawTonT(CanvasImpl & canvas, Point dest);
private:
	void alphaBlend(CanvasImpl & canvas, Rect srcrect, Rect destrect);
	void alphaBlendTonT(CanvasImpl & canvas, Rect srcrect, Rect destrect);
};

class BitmapPallete: public Bitmap {
public:	
	BitmapPallete(BITMAPINFO * info, Byte const * buffer, int transparentIndex);

	void drawInto(CanvasImpl & canvas, Point dest) override;
	void drawInto(CanvasImpl & canvas, Rect destrect) override;
	void copyRectInto(CanvasImpl & canvas, Rect srcrect, Point dest) override;
private:
	void drawInto(CanvasImpl & canvas, Rect srcrect, Rect destrect);
};

using BitmapPtr = std::shared_ptr<Bitmap>;

inline BitmapPtr createBitmap(HBITMAP hbmp) { return std::make_shared<Bitmap>(hbmp); }
inline BitmapPtr cloneBitmap(std::shared_ptr<ImageImpl> const & image) { return createBitmap(image->cloneHbitmap()); }

HBITMAP cloneBitmap(HDC dc, HBITMAP hbmp);

std::string info(Ptr const & image);

} // namespace tk::image

std::ostream & operator<<(std::ostream & os, BITMAPINFOHEADER const & bh);

#endif
