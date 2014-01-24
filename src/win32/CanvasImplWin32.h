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
    
    HDC getHDC() const;

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

	virtual void textRect(Rect const & openrect, die::NativeString const & text, TextParams const & params);
    virtual WDims measureText(die::NativeString const & text);

	void restoreObjects();
};

class CanvasImplWin : public CanvasImpl {
	HWND hWnd;
public:
    CanvasImplWin();
	CanvasImplWin(CanvasImplWin && temp);
	virtual ~CanvasImplWin();

	void getDC(HWND hWnd);
	void releaseDC();
};

}

#endif
