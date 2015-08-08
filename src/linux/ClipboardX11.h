#ifndef CLIPBOARDX11_H_DIE_TK_LINUX_DIEGO_2015_AUG_06
#define CLIPBOARDX11_H_DIE_TK_LINUX_DIEGO_2015_AUG_06

#include <X11/Xlib.h>
#include "../NativeString.h"
#include "ScopedX11.h"
#include <memory>

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
	struct Property {
		struct XFreeFO {
			void operator()(unsigned char * ptr) { if( ptr ) XFree(ptr); }
		};
		std::unique_ptr<unsigned char, XFreeFO> data;
		int format;
		unsigned long nitems;
		Atom type;
		Property() = default;
		Property(::Window windowId, Atom property);
		Property(Property &&) = default;
		explicit operator bool() const { return data.operator bool(); }
	};

	Property readPropertyFromSel(XEvent const & e);
	XEvent checkEvent();
};

}

#endif
