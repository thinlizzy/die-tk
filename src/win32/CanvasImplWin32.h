#ifndef CANVAS_IMPL_WIN32_H_jd324894fhg0ln23hfd
#define CANVAS_IMPL_WIN32_H_jd324894fhg0ln23hfd

#include <windows.h>
#include "../Canvas.h"

namespace tk {

class CanvasImpl : public Canvas {
    HPEN hpenOrig;
    HBRUSH hbrushOrig;
protected:
	HDC dc;
	CanvasImpl();
public:
	explicit CanvasImpl(HDC dc);
	CanvasImpl(CanvasImpl && temp);
	virtual ~CanvasImpl();

	virtual void setPen(Pen const & pen);
	virtual void setBrush(Brush const & brush);

	virtual void translate(Point p);
	virtual void clearTranslate();
	virtual void addClipRect(Rect const & openrect);
	virtual void clearClipping();

	virtual void plot(Point p, RGBColor const & color);
	virtual void drawLine(Point p1, Point p2);
	virtual void drawPoly(Points const & polygon);

	virtual void rectangle(Rect const & rect);
	virtual void fillRect(Rect const & openrect);

	void fillRect(RECT rect);

	virtual void drawImage(ImageRef ih, Point start);
	virtual void drawImage(ImageRef ih, Rect destrect);

    void drawImage(Point dest, unsigned char const buffer[], BITMAPINFO * info);
    void drawImage(Rect const & destrect, unsigned char const buffer[], BITMAPINFO * info);
    void drawImage(Point start, unsigned char const buffer[], ImageType type, WDims dim);
    void drawImage(Point start, unsigned char const buffer[], ImageType type, WDims dim, WDims dest);

	virtual void textRect(Rect const & openrect, std::string const & text, TextParams const & params);

	void restoreObjects();
};

class CanvasImplWin : public CanvasImpl {
	HWND hWnd;
public:
    CanvasImplWin();
	explicit CanvasImplWin(HWND hWnd);
	CanvasImplWin(CanvasImplWin && temp);
	virtual ~CanvasImplWin();

	void getDC(HWND hWnd);
	void releaseDC();
};

}

#endif
