#ifndef NATIVE_CONTROL_WIN_dfjj4395gj49fdgn4
#define NATIVE_CONTROL_WIN_dfjj4395gj49fdgn4

#include <windows.h>

#include "../../Window.h"
#include "../../ControlParams.h"
#include "../../WindowObjects.h"

#include "../ScopedObjects.h"
#include "../CanvasImplWin32.h"
#include "../../util/optional.h"
#include <NativeString.h>

namespace tk {

class NativeControlImpl {
private:
   	Cursor cursor;
   	RGBColor backgroundColor;
    bool trackingMouse;
protected:
	Rect rect_;
	CanvasImplWin canvasImpl;

	NativeControlImpl();
    
	scoped::DC getDC();
	SIZE getTextDims();
public:
	HWND hWnd;

	NativeControlImpl(Window & parent, ControlParams const & params, wchar_t const classname[], DWORD style);
    NativeControlImpl(NativeControlImpl const &) = delete;
    NativeControlImpl & operator=(NativeControlImpl const &) = delete;
    
	virtual ~NativeControlImpl() = 0;
	virtual optional<LRESULT> processMessage(UINT message, WPARAM & wParam, LPARAM & lParam);
    virtual optional<LRESULT> processNotification(UINT message, UINT notification, WPARAM wParam, LPARAM lParam);

	virtual Rect rect() const;

	virtual void setPos(Point pos);
	virtual void setDims(WDims dims);
	virtual void setRect(Rect rect);

	virtual die::NativeString getText() const;
	virtual void setText(die::NativeString const & text);
    
	bool enabled() const;
	void enable();
	void disable();

	bool visible() const;
	void show();
	void hide();
	void bringToFront();
	void sendToBack();

	Canvas & canvas();
	void repaint();

	void setCursor(Cursor cursor);
	void setBackground(RGBColor const & color);

	Point screenToClient(Point const & point) const;

	HandleMouseEvent onMouse(HandleMouseEvent callback);

	ProcessKeyEvent onKey(ProcessKeyEvent callback);
	ProcessKeypress onKeypress(ProcessKeypress callback);

	HandlePaint onPaint(HandlePaint callback);
private:
    optional<LRESULT> doMouseEvent(UINT message, WPARAM wParam, LPARAM lParam, bool firstEnter);
};

}

#endif
