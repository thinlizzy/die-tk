#include "WindowImplX11.h"
#include <memory>
#include <X11/Xatom.h>
#include "ResourceManager.h"
#include "ConvertersX11.h"
#include "Property.h"
#include "../src/CallbackUtils.h"
#include "../src/log.h"

namespace {

tk::ResourceManagerSingleton resourceManager;

::Window createWindow(tk::WindowParams const & params)
{
	int x,y;
	tk::WDims dims;
	if( params.isDefaultPos() ) {
		x = y = 0;
	} else {
		x = params.start_.x;
		y = params.start_.y;
	}
	if( params.isDefaultDims() ) {
		dims.width = dims.height = 200; // TODO find a correct default
	} else {
		dims = params.dims_;
	}
	return resourceManager->createTopLevelWindow(x, y, dims.width, dims.height);
}

}

namespace tk {

template<typename T> using WindowCallbackMap = CallbackMap<WindowImpl *, T>;

WindowCallbackMap<AllowOperation> cbClose;
WindowCallbackMap<ProcessResize> cbResize;
WindowCallbackMap<HandleResize> cbAfterResize;
WindowCallbackMap<HandleEvent> cbUserEvent;

Atom WM_DELETE_WINDOW = XInternAtom(resourceManager->dpy, "WM_DELETE_WINDOW", False);

WindowImpl::WindowImpl(WindowParams const & params):
	NativeControlImpl(resourceManager->root(),createWindow(params))
{
	XSetWMProtocols(resourceManager->dpy, windowId, &WM_DELETE_WINDOW, 1);

	XSelectInput(resourceManager->dpy, windowId,
			KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask |
			EnterWindowMask | LeaveWindowMask | PointerMotionMask |
			ExposureMask | StructureNotifyMask | PropertyChangeMask
			);
	// TODO set do-not-propagate-mask for KeyPress and KeyRelease events

	setText(params.text_);
	XSetIconName(resourceManager->dpy, windowId, params.text_.str.c_str());

	if( params.initialState & ws_visible ) {
		show();
	}
	if( params.initialState & ws_maximized ) {
		maximize(true);
	}
	if( params.initialState & ws_minimized ) {
		XIconifyWindow(resourceManager->dpy, windowId, DefaultScreen(resourceManager->dpy));
	}
}

WindowImpl::~WindowImpl()
{
    removeFromCb(this,cbClose);
    removeFromCb(this,cbResize);
    removeFromCb(this,cbUserEvent);

	XDestroyWindow(resourceManager->dpy, windowId);
}

int WindowImpl::state() const
{
	int result = 0;
	if( visible() ) result |= ws_visible;

	Atom actual_type;
	int actual_format;
	unsigned long num_items, bytes_after;
	Atom * atoms = NULL;
	XGetWindowProperty(resourceManager->dpy, windowId,
		XInternAtom(resourceManager->dpy, "_NET_WM_STATE", False),
		0, 1024, False, XA_ATOM, &actual_type, &actual_format, &num_items, &bytes_after, (unsigned char**)&atoms);
    for( unsigned long i=0; i<num_items; ++i ) {
        if( atoms[i] == XInternAtom(resourceManager->dpy, "_NET_WM_STATE_HIDDEN", False) ) {
        	result |= ws_minimized;
        } else
		if( atoms[i] == XInternAtom(resourceManager->dpy, "_NET_WM_STATE_MAXIMIZED_HORZ", False) ) {
			result |= ws_maximized;
		}
    }
	XFree(atoms);

	return result;
}

void WindowImpl::setBorders(bool value)
{
	XSetWindowAttributes attr;
	attr.override_redirect = value ? False : True;
	XChangeWindowAttributes(resourceManager->dpy, windowId, CWOverrideRedirect, &attr);
}

void WindowImpl::registerControl(std::shared_ptr<NativeControlImpl> control)
{
	controls[control->windowId] = control;
	ResourceManagerSingleton resourceManager;
	resourceManager->registerControl(control);
}

void WindowImpl::unregisterControl(std::shared_ptr<NativeControlImpl> control)
{
    if( controls.erase(control->windowId) > 0 ) {
    	ResourceManagerSingleton resourceManager;
    	resourceManager->unregisterControl(control);
        XDestroyWindow(resourceManager->dpy, control->windowId);
    }
}

NativeString WindowImpl::getText() const
{
	char * windowName;
	XFetchName(resourceManager->dpy, windowId, &windowName);
	if( windowName == nullptr ) return {};

	auto windowNamePtr = std::unique_ptr<void,int (*)(void*)>(static_cast<void *>(windowName),&XFree);
	NativeString result;
	result.str.assign(windowName);
	return result;
}

void WindowImpl::setText(NativeString const & text)
{
	XStoreName(resourceManager->dpy, windowId, text.str.c_str());
}

// callbacks & messages

AllowOperation WindowImpl::onClose(AllowOperation callback)
{
    return setCallback(this,cbClose,callback);
}

ProcessResize WindowImpl::onResize(ProcessResize callback)
{
    return setCallback(this,cbResize,callback);
}

HandleResize WindowImpl::afterResize(HandleResize callback)
{
    return setCallback(this,cbAfterResize,callback);
}

HandleEvent WindowImpl::onUserEvent(HandleEvent callback)
{
    return setCallback(this,cbUserEvent,callback);
}

void WindowImpl::processMessage(XEvent & e)
{
	//log::info("window ",e.xany.window," got event ",xEventToStr(e.type));

	switch(e.type) {
		case ClientMessage: {
			auto atom = Atom(e.xclient.data.l[0]);
			if( atom == WM_DELETE_WINDOW ) {
				auto canClose = findExec(this,cbClose);
				if( canClose && *canClose == false ) return;

				hide();
			} else {
				executeCallback(this,cbUserEvent,UserEvent{atom,&e.xclient});
			}
		} break;

		case ConfigureNotify: {
			if( ignoreConfigureNotify > 0 ) {
				--ignoreConfigureNotify;
				break;
			}
			auto & data = e.xconfigure;
			WDims newDims(data.width,data.height);

			if( newDims != cachedDims ) {
				cachedDims = newDims;
				auto resNewDims = findExec(this,cbResize,newDims);
				if( resNewDims && *resNewDims != newDims ) {
					// invalidate cache to make the event to trigger again. because WMs
					cachedDims = WDims();
					if( maximized() ) {
						++ignoreConfigureNotify;
						maximize(false);
					}
					setDims(*resNewDims);
				} else {
					executeCallback(this, cbAfterResize, newDims);
				}
			}
		} break;

		default:
			NativeControlImpl::processMessage(e);
	}
}

bool WindowImpl::maximized() const
{
	Property p{windowId,XInternAtom(resourceManager->dpy,"_NET_WM_STATE", False)};
	return p.hasItem("_NET_WM_STATE_MAXIMIZED_HORZ");   // it will have VERT as well
}

void WindowImpl::maximize(bool yes)
{
	XEvent xev{};
	xev.type = ClientMessage;
	xev.xclient.window = windowId;
	xev.xclient.message_type = XInternAtom(resourceManager->dpy, "_NET_WM_STATE", False);
	xev.xclient.format = 32;
	xev.xclient.data.l[0] = yes ? 1: 0;
	xev.xclient.data.l[1] = XInternAtom(resourceManager->dpy,"_NET_WM_STATE_MAXIMIZED_HORZ", False);
	xev.xclient.data.l[2] = XInternAtom(resourceManager->dpy,"_NET_WM_STATE_MAXIMIZED_VERT", False);
	XSendEvent(resourceManager->dpy, resourceManager->root(),
		False, SubstructureNotifyMask, &xev);
}

}
