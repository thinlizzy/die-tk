#ifndef WINDOW_H_jdfsr89f4wrjfdasfsd
#define WINDOW_H_jdfsr89f4wrjfdasfsd

#include <memory>
#include "WindowObjects.h"
#include "controls/Control.h"

namespace tk {

class Window: public virtual Control {
public:
	virtual ~Window() {};

	virtual int state() const = 0;
};

}

#endif

