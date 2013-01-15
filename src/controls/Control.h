#ifndef CONTROL_H_gfh430hm4bg73bg93gllhr52bzq
#define CONTROL_H_gfh430hm4bg73bg93gllhr52bzq

#include "../WindowObjects.h"
#include <memory>
#include "../Callbacks.h"

namespace tk {

class Canvas;

// base class for all widgets including windows
// TODO think on renaming it to Widget or Surface

class Control: public std::enable_shared_from_this<Control> {
public:
	virtual ~Control() {}

	virtual int x() const = 0;
	virtual int y() const = 0;
	virtual Point pos() const = 0;
	virtual int width() const = 0;
	virtual int height() const = 0;
	virtual WDims dims() const = 0;
	virtual Rect rect() const = 0;
	virtual void setPos(Point pos) = 0;
	virtual void setDims(WDims dims) = 0;
	virtual void setRect(Rect rect) = 0;

	virtual bool enabled() const = 0;
	virtual void enable() = 0;
	virtual void disable() = 0;

	virtual bool visible() const = 0;
	virtual void show() = 0;
	virtual void hide() = 0;
	virtual void bringToFront() = 0;
	virtual void sendToBack() = 0;

	virtual Canvas & canvas() = 0;
	virtual void repaint() = 0;

	virtual void setCursor(Cursor cursor) = 0;
	virtual void setBackground(RGBColor const & color = RGBColor()) = 0;

	virtual void clear(RGBColor const & color = RGBColor()) = 0;

	virtual Point screenToClient(Point const & point) const = 0;
protected:
    // most used callbacks
	void onMouse(HandleMouseEvent callback);    // all
	void onKey(ProcessKeyEvent callback);       // edit, memo
	void onKeypress(ProcessKeypress callback);  // edit, memo
	void onClick(HandleOperation callback);     // buttons, checkbox
    // seldom used callbacks
	void onPaint(HandlePaint callback);       // window and paintbox
	void onChange(HandleOperation callback);  // combobox
};

}

#endif

