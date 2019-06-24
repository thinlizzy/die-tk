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

	void translate(Point p) override;
	void clearTranslate() override;
	void addClipRect(Rect const & openrect) override;
	void clearClipping() override;

	void plot(Point p, RGBColor const & color) override;
	void drawLine(Point p1, Point p2, Pen const & pen) override;
	void drawPoly(Points const & polygon, Pen const & pen) override;

	void rectangle(Rect const & rect, Pen const & pen) override;
	void fillRect(Rect const & openrect, Brush const & brush) override;
	void roundRect(Rect const & openrect, Brush const & brush, WDims ellipseDims) override;

    // TODO use some sort of caching on pens and brushes
	void setPen(Pen const & pen);
	void setBrush(Brush const & brush);

	void drawText(Point p, NativeString const & text, RGBColor const & color) override;
	void drawText(Point p, NativeString const & text, RGBColor const & textColor, RGBColor const & backgroundColor) override;
	void textRect(Rect const & openrect, NativeString const & text, TextParams const & params) override;
    WDims measureText(NativeString const & text) override;

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
