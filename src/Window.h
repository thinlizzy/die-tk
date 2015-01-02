#ifndef WINDOW_H_jdfsr89f4wrjfdasfsd
#define WINDOW_H_jdfsr89f4wrjfdasfsd

#include "Surface.h"
#include "Callbacks.h"
#include "WindowParams.h"

namespace tk {

class Control;
class WindowImpl;

/* this is a scoped movable only object.
 * unlike Control, when it goes out of scope, the window handle is destroyed.
 * it uses a shared_ptr because other operations like getParent() can add more references to the implementation
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

