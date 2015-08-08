// source: https://github.com/edrosten/x_clipboard/blob/master/paste.cc
#include <cstring>
#include <algorithm>
#include <iostream>
#include <X11/Xatom.h>
#include "ResourceManager.h"
#include "ClipboardX11.h"
#include "../log.h"
using std::cerr;
using std::hex;
using std::dec;
using std::endl;

#define DEBUG_CLIP(x)
// #define DEBUG_CLIP(x) cerr << x << endl

namespace {

char const * getAtomName(Display * disp, Atom atom)
{
	if( atom == None ) return "None";
	return XGetAtomName(disp, atom);
}

Atom pick_target(Display* disp, Atom* atom_list, int nitems, char const * datatype)
{
	for(int i=0; i < nitems; i++)
	{
		auto atom_name = getAtomName(disp, atom_list[i]);
		DEBUG_CLIP("Type " << i << " = " << atom_name);

		if( std::strcmp(datatype,atom_name) == 0 ) {
			DEBUG_CLIP("Will request type: " << atom_name);
			return atom_list[i];
		}
	}

	return None;
}

}

namespace tk {

Clipboard::Property::Property(::Window windowId, Atom property)
{
	unsigned long bytes_after;
	unsigned char * ret = 0;

	int read_bytes = 1024;

	//Keep trying to read the property until there are no bytes unread.
	do {
		if( ret != 0 ) {
			XFree(ret);
		}
		XGetWindowProperty(resourceManager.dpy, windowId, property, 0, read_bytes, False, AnyPropertyType,
							&type, &format, &nitems, &bytes_after,
							&ret);

		read_bytes *= 2;
	} while( bytes_after != 0 );
	data.reset(ret);

	DEBUG_CLIP("Actual type: " << getAtomName(resourceManager.dpy, type));
	DEBUG_CLIP("Actual format: " << format);
	DEBUG_CLIP("Number of items: " << nitems);

}

Clipboard::Clipboard(char const * clipProperty):
	sel(XInternAtom(resourceManager.dpy, clipProperty, False)),
	XA_TARGETS(XInternAtom(resourceManager.dpy, "TARGETS", False))
{
	window.reset(WindowImpl::createWindow(0,0,100,100));
	XConvertSelection(resourceManager.dpy, sel, XA_TARGETS, sel, window.get(), CurrentTime);
	XFlush(resourceManager.dpy);
}

Bool clipboardPredicate(Display *, XEvent * event, XPointer arg)
{
	return (event->type == SelectionNotify && event->xany.window == reinterpret_cast<::Window>(arg)) ? True : False;
}

XEvent Clipboard::checkEvent()
{
	XEvent e;
    XIfEvent(resourceManager.dpy, &e, &clipboardPredicate, reinterpret_cast<XPointer>(window.get()));
    DEBUG_CLIP("A selection notify has arrived!");
    DEBUG_CLIP("Requester = 0x" << hex << e.xselectionrequest.requestor << dec);
    DEBUG_CLIP("Target atom    = " << getAtomName(resourceManager.dpy, e.xselection.target));
    DEBUG_CLIP("Property atom  = " << getAtomName(resourceManager.dpy, e.xselection.property));
    DEBUG_CLIP("Selection atom = " << getAtomName(resourceManager.dpy, e.xselection.selection));
	return e;
}

auto Clipboard::readPropertyFromSel(XEvent const & e) -> Property
{
	Atom target = e.xselection.target;
	if( e.xselection.property == None ) {
		log::error(target == XA_TARGETS ?
			"TARGETS can not be converted (nothing owns the selection)" :
			"the selection can not be converted"
		);
		return {};
	}

	return Property(window.get(), sel);
}

NativeString Clipboard::pasteString()
{
	// 1
    XEvent e = checkEvent();

	//If we're being given a list of targets (possible conversions)
	if( e.xselection.target != XA_TARGETS ) {
		log::error("Target ", e.xselection.target, " should be XA_TARGETS ", XA_TARGETS);
		return {};
	}
    auto prop = readPropertyFromSel(e);
    if( ! prop ) {
		return {};
    }

	// request the STRING type
	Atom targetToBeRequested = pick_target(resourceManager.dpy, reinterpret_cast<Atom *>(prop.data.get()), prop.nitems, "STRING");
	if( targetToBeRequested == None ) {
		log::error("No matching datatypes for clipboard!");
		return {};
	}
	DEBUG_CLIP("Now requesting type " << getAtomName(resourceManager.dpy, targetToBeRequested));
	XConvertSelection(resourceManager.dpy, sel, targetToBeRequested, sel, window.get(), CurrentTime);
	XFlush(resourceManager.dpy);

	// 2
    XEvent e2 = checkEvent();

    NativeString result;
	if( e2.xselection.target == targetToBeRequested ) {
	    if( auto prop = readPropertyFromSel(e2) ) {
			result.str.resize(prop.nitems * prop.format/8);
			std::copy_n(prop.data.get(),result.str.size(),result.str.begin());
	    }
	}
	return result;
}

}
