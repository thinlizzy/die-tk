#ifndef WINDOW_H_jdfsr89f4wrjfdasfsd
#define WINDOW_H_jdfsr89f4wrjfdasfsd

#include <memory>
#include "WindowObjects.h"
#include "controls/Control.h"
#include "Callbacks.h"

namespace tk {

class Window: public virtual Control {
public:
	virtual ~Window() {};

	virtual int state() const = 0;
    
    using Control::onMouse;
    using Control::onPaint;
	void onClose(AllowOperation callback);
	void onResize(ProcessResize callback);
	void onUserEvent(HandleEvent callback);
};

}

#endif

