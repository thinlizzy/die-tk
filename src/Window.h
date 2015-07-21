#ifndef WINDOW_H_jdfsr89f4wrjfdasfsd
#define WINDOW_H_jdfsr89f4wrjfdasfsd

#include "Surface.h"
#include "Callbacks.h"
#include "WindowParams.h"

namespace tk {

class Control;
class WindowImpl;

/*
 * This is a scoped movable only object.
 * Unlike Control, when it goes out of scope, the window handle is destroyed.
 * However, other operations like Control::getParent() can add more references to the implementation, thus extending the lifetime of the Window.
 */
class Window: public Surface {
public:
    Window(WindowParams const & params);
    Window(Window &&) = default;
    Window & operator=(Window &&) = default;
	virtual ~Window();     // all controls are destroyed too

	int state() const;
    
    void remove(Control & control);
    
    void addBorders() { setBorders(true); }
    void removeBorders() { setBorders(false); }
    void setBorders(bool value);
    
    using Surface::onPaint;
    using Surface::onKeyDown;
    using Surface::onKeyUp;
    using Surface::onKeypress;
	AllowOperation onClose(AllowOperation callback);
	ProcessResize onResize(ProcessResize callback);
	HandleEvent onUserEvent(HandleEvent callback);
    
    // internal use only
    Window(std::shared_ptr<WindowImpl> impl);
    WindowImpl & getImpl();     
    WindowImpl const & getImpl() const;
    std::shared_ptr<WindowImpl> getImplPtr();
};

}

#endif

