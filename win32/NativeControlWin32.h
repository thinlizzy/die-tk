#ifndef NATIVE_CONTROL_WIN_dfjj4395gj49fdgn4
#define NATIVE_CONTROL_WIN_dfjj4395gj49fdgn4

#include <windows.h>

#include "../src/Window.h"
#include "../src/ControlParams.h"
#include "../src/WindowObjects.h"
#include "../src/util/optional.h"
#include "../src/CallbackUtils.h"
#include "../src/NativeString.h"

#include "ScopedObjects.h"
#include "CanvasImplWin32.h"

namespace tk {

class NativeControlImpl;
template<typename T> using ControlCallbackMap = CallbackMap<NativeControlImpl *, T>; 

class NativeControlImpl {
private:
   	Cursor cursor;
   	RGBColor backgroundColor;
    bool trackingMouse;
protected:
	CanvasImplWin canvasImpl;
    scoped::Brush backgroundBrush;

	NativeControlImpl();
    
	scoped::DC getDC() const;
	SIZE getTextDims();
	RECT getClientRect() const;
public:
	HWND hWnd;

	NativeControlImpl(HWND parentHwnd, ControlParams const & params, wchar_t const classname[], DWORD style);
    NativeControlImpl(NativeControlImpl const &) = delete;
    NativeControlImpl & operator=(NativeControlImpl const &) = delete;    
	virtual ~NativeControlImpl() = 0;
    virtual NativeControlImpl * clone() const = 0;
    
	virtual optional<LRESULT> processMessage(UINT message, WPARAM & wParam, LPARAM & lParam);
    virtual optional<LRESULT> processNotification(UINT message, UINT notification, WPARAM wParam, LPARAM lParam);

	virtual Rect rect() const;

	virtual void setPos(Point pos);
	virtual void setDims(WDims dims);
	virtual void setRect(Rect rect);

	virtual NativeString getText() const;
	virtual void setText(NativeString const & text);
    
    virtual ClipboardType copyToClipboard() const;
    
	bool enabled() const;
	void enable();
	void disable();

	bool visible() const;
	virtual void show();
	virtual void hide();
	void bringToFront();
	void sendToBack();

	Canvas & canvas();
	virtual void repaint();

	void setCursor(Cursor cursor);
	void setBackground(RGBColor const & color);

	Point screenToClient(Point const & point) const;
    
    HWND getParentHandle() const;
    
    ControlParams getControlData() const;

    HandleMouseButton onMouseDown(HandleMouseButton callback);
    HandleMouseButton onMouseUp(HandleMouseButton callback);
    HandleMouseMove onMouseEnter(HandleMouseMove callback);
    HandleMouseMove onMouseOver(HandleMouseMove callback);
    HandleMouseMove onMouseLeave(HandleMouseMove callback);

	ProcessKeyEvent onKeyDown(ProcessKeyEvent callback);
	ProcessKeyEvent onKeyUp(ProcessKeyEvent callback);
	ProcessKeypress onKeypress(ProcessKeypress callback);

	HandlePaint onPaint(HandlePaint callback);
private:
    optional<LRESULT> doMouseButton(ControlCallbackMap<HandleMouseButton> & callbacks, UINT message, WPARAM wParam, LPARAM lParam);
    optional<LRESULT> doMouseMove(ControlCallbackMap<HandleMouseMove> & callbacks, LPARAM lParam);
};

}

#endif
