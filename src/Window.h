#ifndef WINDOW_H_jdfsr89f4wrjfdasfsd
#define WINDOW_H_jdfsr89f4wrjfdasfsd

#include "Control.h"
#include "Callbacks.h"
#include "WindowParams.h"

namespace tk {

class WindowImpl;
    
class Window: public Control {
public:
    Window(WindowParams const & params = WindowParams());
	virtual ~Window();

	int state();
    
    void remove(Control & control);
    
    using Control::onMouse;
    using Control::onPaint;
    using Control::onKey;
    using Control::onKeypress;
	AllowOperation onClose(AllowOperation callback);
	ProcessResize onResize(ProcessResize callback);
	HandleEvent onUserEvent(HandleEvent callback);
    
    WindowImpl & getImpl();     // internal use only
protected:
    Window(std::shared_ptr<WindowImpl> impl);  // internal use only
};

}

#endif

