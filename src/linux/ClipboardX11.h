#ifndef CLIPBOARDX11_H_DIE_TK_LINUX_DIEGO_2015_AUG_06
#define CLIPBOARDX11_H_DIE_TK_LINUX_DIEGO_2015_AUG_06

#include <X11/Xlib.h>
#include "../NativeString.h"
#include "ScopedX11.h"
#include "Property.h"

namespace tk {

class Clipboard {
	Atom sel;
	Atom XA_TARGETS;
	scoped::Window window;
public:
	explicit Clipboard(char const * clipProperty);
	NativeString pasteString();
	// TODO get image data too
private:
	Property readPropertyFromSel(XEvent const & e);
	XEvent checkEvent();
};

}

#endif
