#ifndef NATIVE_CONTROL_WIN_dfjj4395gj49fdgn4
#define NATIVE_CONTROL_WIN_dfjj4395gj49fdgn4

#include "../../ControlParams.h"
#include "../../WindowObjects.h"
#include "../../controls/Control.h"

#include <windows.h>
#include "../ScopedObjects.h"
#include "../CanvasImplWin32.h"
#include "../../util/optional.h"

namespace tk {

class NativeControlImpl: public virtual Control {
private:
   	Cursor cursor;
   	RGBColor backgroundColor;
protected:
	Rect rect_;
	CanvasImplWin canvasImpl;

	NativeControlImpl();
	scoped::DC getDC();
	std::string getText() const;
	void setText(std::string const & text);
	SIZE getTextDims();
public:
	HWND hWnd;

	NativeControlImpl(HWND parent_hWnd, ControlParams const & params, char const classname[], DWORD style);

	virtual ~NativeControlImpl() = 0;
	virtual optional<LRESULT> processMessage(UINT message, WPARAM & wParam, LPARAM & lParam);
    virtual optional<LRESULT> processNotification(UINT message, UINT notification, WPARAM wParam, LPARAM lParam);

	virtual int x() const;
	virtual int y() const;
	virtual Point pos() const;
	virtual int width() const;
	virtual int height() const;
	virtual WDims dims() const;
	virtual Rect rect() const;

	virtual void setPos(Point pos);
	virtual void setDims(WDims dims);
	virtual void setRect(Rect rect);

	virtual bool enabled() const;
	virtual void enable();
	virtual void disable();

	virtual bool visible() const;
	virtual void show();
	virtual void hide();
	virtual void bringToFront();
	virtual void sendToBack();

	virtual Canvas & canvas();
	virtual void repaint();

	virtual void setCursor(Cursor cursor);
	virtual void setBackground(RGBColor const & color);

	virtual void clear(RGBColor const & color);

	virtual Point screenToClient(Point const & point) const;
};

}

#endif
