#ifndef CONTROL_H_gfh430hm4bg73bg93gllhr52bzq
#define CONTROL_H_gfh430hm4bg73bg93gllhr52bzq

#include <memory>
#include <string>
#include "WindowObjects.h"
#include "Callbacks.h"
#include "ControlParams.h"

namespace tk {

class Window;
class Canvas;
class NativeControlImpl;

// base class for all widgets including windows

class Control {
public:
    Control() = default;
    Control(Control const &) = delete;              // TODO delegate to NativeControlImpl->clone()
    Control & operator=(Control const &) = delete;  // TODO same
    Control(Control &&) = default;
    Control & operator=(Control &&) = default;
    virtual ~Control() = 0;
    
	int x() const;
	int y() const;
	Point pos() const;
	int width() const;
	int height() const;
	WDims dims() const;
	Rect rect() const;

	void setPos(Point pos);
	void setDims(WDims dims);
	void setRect(Rect rect);

	void bringToFront();
	void sendToBack();
    
	bool enabled() const;
	void enable();
	void disable();
    void setEnabled(bool enabled);

	bool visible() const;
	void show();
	void hide();
    void setVisible(bool visible);    

	Canvas & canvas();
	void repaint();

	void setCursor(Cursor cursor);
	void setBackground(RGBColor const & color);

	void clear(RGBColor const & color = RGBColor());

	Point screenToClient(Point const & point) const;
protected:
    std::shared_ptr<NativeControlImpl> impl;

	std::string getText() const;
	void setText(std::string const & text);
    
    // most used callbacks
	HandleMouseEvent onMouse(HandleMouseEvent callback);    // all
	ProcessKeyEvent onKey(ProcessKeyEvent callback);       // edit, memo
	ProcessKeypress onKeypress(ProcessKeypress callback);  // edit, memo
    // seldom used callbacks
	HandlePaint onPaint(HandlePaint callback);       // window and paintbox
};

}

#endif

