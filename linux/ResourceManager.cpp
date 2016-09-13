#include "ResourceManager.h"
#include "../src/log.h"

namespace {
std::shared_ptr<tk::NativeControlImpl> nullControl;
std::shared_ptr<tk::WindowImpl> nullWindow;
}

namespace tk {

ResourceManager::ResourceManager():
	dpy(XOpenDisplay(0))
{
	if( ! dpy ) {
		log::error("Could not open X11 display!");
	}
}

ResourceManager::~ResourceManager()
{
	 XCloseDisplay(dpy);
}

void ResourceManager::processMessages()
{
	while( XPending(dpy) > 0 ) {
	    XEvent e;
	    XNextEvent(dpy, &e);
	    switch(e.type) {
	    case GraphicsExpose:
	    case NoExpose:
	    	log::info("unhandled event ",e.type);
	    	break;
	    default:
	    	if( auto control = findWindowedControl(e.xany.window) ) {
	    		control->processMessage(e);
	    	}
	    }
	}
}

void ResourceManager::registerControl(std::shared_ptr<NativeControlImpl> const & control)
{
	controlMap[control->windowId] = control;
}

void ResourceManager::unregisterControl(std::shared_ptr<NativeControlImpl> const & control)
{
	controlMap.erase(control->windowId);
}

void ResourceManager::registerClonedControl(std::shared_ptr<NativeControlImpl> const & control)
{
	findWindow(control->getParentHandle())->registerControl(control);
}

void ResourceManager::registerWindow(std::shared_ptr<WindowImpl> const & window)
{
	controlMap[window->windowId] = window;
	windowMap[window->windowId] = window;
}

void ResourceManager::unregisterWindow(std::shared_ptr<WindowImpl> const & window)
{
	controlMap.erase(window->windowId);
	windowMap.erase(window->windowId);
}

std::shared_ptr<WindowImpl> ResourceManager::findWindow(::Window windowId)
{
	auto it = windowMap.find(windowId);
    if( it == windowMap.end() ) return nullWindow;
    if( auto spt = it->second.lock() ) return spt;
    windowMap.erase(it);
    return nullWindow;
}

std::shared_ptr<NativeControlImpl> ResourceManager::findWindowedControl(::Window windowId)
{
	auto it = controlMap.find(windowId);
    if( it == controlMap.end() ) return nullControl;
    if( auto spt = it->second.lock() ) return spt;
    controlMap.erase(it);
    return nullControl;
}

::Window ResourceManager::root()
{
	return DefaultRootWindow(dpy);
}

char const * ResourceManager::getAtomName(Atom atom)
{
	if( atom == None ) return "None";
	return XGetAtomName(dpy, atom);
};

::Window ResourceManager::createTopLevelWindow(int x, int y, int width, int height)
{
	int borderColor = BlackPixel(dpy, DefaultScreen(dpy));
	int backgroundColor = borderColor;
	int borderWidth = 0;
	return XCreateSimpleWindow(dpy,root(),
		x, y, width, height,
		borderWidth, borderColor, backgroundColor);
}

}
