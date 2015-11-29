// adapted from https://github.com/edrosten/x_clipboard/blob/master/paste.cc
#include "ClipboardX11.h"
#include <algorithm>
#include <iostream>
#include <X11/Xatom.h>
#include "ResourceManager.h"
#include "../src/log.h"
using std::cerr;
using std::hex;
using std::dec;
using std::endl;

#define DEBUG_CLIP(x)
// #define DEBUG_CLIP(x) cerr << x << endl

namespace {

tk::ResourceManagerSingleton resourceManager;

}

namespace tk {

Clipboard::Clipboard(char const * clipProperty):
	sel(XInternAtom(resourceManager->dpy, clipProperty, False)),
	XA_TARGETS(XInternAtom(resourceManager->dpy, "TARGETS", False))
{
	window.reset(WindowImpl::createWindow(0,0,100,100));
	XConvertSelection(resourceManager->dpy, sel, XA_TARGETS, sel, window.get(), CurrentTime);
	XFlush(resourceManager->dpy);
}

Bool clipboardPredicate(Display *, XEvent * event, XPointer arg)
{
	return (event->type == SelectionNotify && event->xany.window == reinterpret_cast<::Window>(arg)) ? True : False;
}

XEvent Clipboard::checkEvent()
{
	XEvent e;
    XIfEvent(resourceManager->dpy, &e, &clipboardPredicate, reinterpret_cast<XPointer>(window.get()));
    DEBUG_CLIP("A selection notify has arrived!");
    DEBUG_CLIP("Requester = 0x" << hex << e.xselectionrequest.requestor << dec);
    DEBUG_CLIP("Target atom    = " << getAtomName(resourceManager->dpy, e.xselection.target));
    DEBUG_CLIP("Property atom  = " << getAtomName(resourceManager->dpy, e.xselection.property));
    DEBUG_CLIP("Selection atom = " << getAtomName(resourceManager->dpy, e.xselection.selection));
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

	// If we're being given a list of targets (possible conversions)
	if( e.xselection.target != XA_TARGETS ) {
		log::error("Target ", e.xselection.target, " should be XA_TARGETS ", XA_TARGETS);
		return {};
	}
    auto prop = readPropertyFromSel(e);
    if( ! prop ) {
		return {};
    }

	// request the STRING type
	Atom targetToBeRequested = prop.get("STRING");
	if( targetToBeRequested == None ) {
		log::error("No matching datatypes for clipboard!");
		return {};
	}
	DEBUG_CLIP("Now requesting type " << resourceManager.getAtomName(targetToBeRequested));
	XConvertSelection(resourceManager->dpy, sel, targetToBeRequested, sel, window.get(), CurrentTime);
	XFlush(resourceManager->dpy);

	// 2
    XEvent e2 = checkEvent();

    NativeString result;
	if( e2.xselection.target == targetToBeRequested ) {
	    if( auto prop = readPropertyFromSel(e2) ) {
			result.str.resize(prop.charSize());
			std::copy_n(prop.charData(),result.str.size(),result.str.begin());
	    }
	}
	return result;
}

}
