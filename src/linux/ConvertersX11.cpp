#include "ConvertersX11.h"
#include <X11/Xlib.h>

#define XES(et) case et: return #et

namespace tk {

std::string xEventToStr(int eventType)
{
	switch(eventType) {
	case 0: return "error?";
	case 1: return "reply?";
	XES(KeyPress);
	XES(KeyRelease);
	XES(ButtonPress);
	XES(ButtonRelease);
	XES(MotionNotify);
	XES(EnterNotify);
	XES(LeaveNotify);
	XES(FocusIn);
	XES(FocusOut);
	XES(KeymapNotify);
	XES(Expose);
	XES(GraphicsExpose);
	XES(NoExpose);
	XES(VisibilityNotify);
	XES(CreateNotify);
	XES(DestroyNotify);
	XES(UnmapNotify);
	XES(MapNotify);
	XES(MapRequest);
	XES(ReparentNotify);
	XES(ConfigureNotify);
	XES(ConfigureRequest);
	XES(GravityNotify);
	XES(ResizeRequest);
	XES(CirculateNotify);
	XES(CirculateRequest);
	XES(PropertyNotify);
	XES(SelectionClear);
	XES(SelectionRequest);
	XES(SelectionNotify);
	XES(ColormapNotify);
	XES(ClientMessage);
	XES(MappingNotify);
	XES(GenericEvent);
	}

	return "unknown event " + std::to_string(eventType);
}

MouseEvent toMouseEvent(XButtonEvent event)
{
	MouseEvent result;
	switch(event.button) {
	case Button1:
		result.button = mb_left;
		break;
	case Button2:
		result.button = mb_right;
		break;
	case Button3:
		result.button = mb_middle;
		break;
	default:
		result.button = MouseButton();
	}

	return result;
}

}

