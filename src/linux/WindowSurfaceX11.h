#ifndef WINDOWSURFACEX11_H_DIE_TK_2014_SEP_7
#define WINDOWSURFACEX11_H_DIE_TK_2014_SEP_7

#include "../objects/Rect.h"
#include "../objects/Color.h"
#include "../ControlParams.h"
#include "../Canvas.h"
#include "../Callbacks.h"
#include "../CallbackUtils.h"
#include <X11/Xlib.h>

namespace tk {

// TODO rename to WindowSurface in all build trees

class NativeControlImpl;
template<typename T> using ControlCallbackMap = CallbackMap<NativeControlImpl *, T>;

class NativeControlImpl {
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

	//

	void bringToFront();
	void sendToBack();

	bool enabled() const;
	void enable();
	void disable();

	virtual die::NativeString getText() const;
	virtual void setText(die::NativeString const & text);

    virtual ClipboardType copyToClipboard() const;

	Canvas & canvas();
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

#endif /* WINDOWSURFACEX11_H_ */
