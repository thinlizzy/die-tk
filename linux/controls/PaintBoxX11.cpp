#include "PaintBoxX11.h"

namespace tk {

PaintBoxX11::PaintBoxX11(::Window parent, ControlParams const & params):
	NativeControlImpl(parent,params,
		KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask |
		EnterWindowMask | LeaveWindowMask | PointerMotionMask | ExposureMask)
{
}

PaintBoxX11 * PaintBoxX11::clone() const
{
	return new PaintBoxX11(getParentHandle(),getControlData());
}

}
