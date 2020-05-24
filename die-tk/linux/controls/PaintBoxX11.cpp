#include "PaintBoxX11.h"
#include <X11/X.h>
#include "../ResourceManager.h"

namespace tk {

PaintBoxX11::PaintBoxX11(::Window parent, ControlParams const & params):
	NativeControlImpl(parent,params,
		ButtonPressMask | ButtonReleaseMask |
		EnterWindowMask | LeaveWindowMask | PointerMotionMask | ExposureMask)
{
}

PaintBoxX11 * PaintBoxX11::clone() const
{
	return new PaintBoxX11(getParentHandle(),getControlData());
}

bool PaintBoxX11::transparent() const
{
	return ! static_cast<bool>(backgroundColor);
}

void PaintBoxX11::setTransparentBackground()
{
	ResourceManagerSingleton resourceManager;
	XSetWindowBackgroundPixmap(resourceManager->dpy, windowId, None);
	XSetWindowBackground(resourceManager->dpy, windowId, None);
	backgroundColor = {};
	// TODO process Expose to redraw parent's background instead?
}

}
