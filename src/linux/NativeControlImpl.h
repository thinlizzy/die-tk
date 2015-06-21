#ifndef NATIVECONTROLIMPLX11_H_DIE_TK_2014_SEP_7
#define NATIVECONTROLIMPLX11_H_DIE_TK_2014_SEP_7

#include "../objects/Rect.h"
#include "../objects/Color.h"
#include "../ControlParams.h"
#include "../Canvas.h"
#include "../Callbacks.h"
#include "../CallbackUtils.h"
#include "ScopedX11.h"
#include <X11/Xlib.h>

namespace tk {

class NativeControlImpl;
template<typename T> using ControlCallbackMap = CallbackMap<NativeControlImpl *, T>;

// this is a base class for window implementations and for window-based controls
class NativeControlImpl {
private:
	bool windowEnabled = true;  // poorman state control. check if there is a native way to enable/disable windows
	scoped::Cursor nativeCursor;
protected:
	NativeControlImpl() = default;
public:
	::Window windowId;

	virtual ~NativeControlImpl();

	void setPos(Point pos);
	void setDims(WDims dims);
	void setRect(Rect rect);

	Rect rect() const;

	bool visible() const;
	void show();
	void hide();

	void bringToFront();
	void sendToBack();

	virtual bool enabled() const;
	virtual void enable();
	virtual void disable();

	virtual die::NativeString getText() const;
	virtual void setText(die::NativeString const & text);

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
};

}

#endif
