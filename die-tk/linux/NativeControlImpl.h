#ifndef NATIVECONTROLIMPLX11_H_DIE_TK_2014_SEP_7
#define NATIVECONTROLIMPLX11_H_DIE_TK_2014_SEP_7

#include <X11/Xlib.h>
#include "ScopedX11.h"
#include "CanvasX11.h"
#include "../src/objects/Rect.h"
#include "../src/objects/Color.h"
#include "../src/ControlParams.h"
#include "../src/Canvas.h"
#include "../src/Callbacks.h"
#include "../src/CallbackUtils.h"
#include "../src/util/optional.h"

namespace tk {

class NativeControlImpl;
template<typename T> using ControlCallbackMap = CallbackMap<NativeControlImpl *, T>;

// this is a base class for window implementations and for window-based controls
class NativeControlImpl {
private:
	scoped::Cursor nativeCursor;
	// TODO check if there is a native way to enable/disable windows
	bool windowEnabled = true;  // poorman state control.

	Cursor cursor = Cursor::defaultCursor;
protected:
	::Window parentWindowId;
	optional<RGBColor> backgroundColor;

	NativeControlImpl(::Window parentWindowId, ::Window windowId);
	NativeControlImpl(::Window parentWindowId, ControlParams const & params, long event_mask);
public:
	::Window windowId;
private:
	CanvasX11 windowCanvas;
public:
	virtual ~NativeControlImpl();

	::Window getWindowId() const { return windowId; }

	void setPos(Point pos);
	void setDims(WDims dims);
	void setRect(Rect rect);

	virtual Rect rect() const;

	bool visible() const;
	void show();
	void hide();

	void bringToFront();
	void sendToBack();

	virtual bool enabled() const;
	virtual void enable();
	virtual void disable();

	virtual NativeString getText() const;
	virtual void setText(NativeString const & text);

    virtual ClipboardType copyToClipboard() const;

	virtual Canvas & canvas();
	void repaint();

	void setCursor(Cursor cursor);
	void setBackground(RGBColor const & color);

	Point screenToClient(Point const & point) const;

	::Window getParentHandle() const;

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

	virtual void processMessage(XEvent & e);
private:
	void keyPressEvent(XEvent & e);
	void keyReleaseEvent(XEvent & e);
};

}

#endif
