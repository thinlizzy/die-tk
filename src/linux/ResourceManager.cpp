#include "ResourceManager.h"
#include "../log.h"

namespace {
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
	    	if( auto windowImpl = findWindow(e.xany.window) ) {
	    		windowImpl->processMessage(e);
	    	}
	    }
	}
}

void ResourceManager::registerWindow(std::shared_ptr<WindowImpl> const & window)
{
	windowMap[window->windowId] = window;
}

void ResourceManager::unregisterWindow(std::shared_ptr<WindowImpl> const & window)
{
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

::Window ResourceManager::root()
{
	return DefaultRootWindow(dpy);
}

char const * ResourceManager::getAtomName(Atom atom)
{
	if( atom == None ) return "None";
	return XGetAtomName(dpy, atom);
};


ResourceManager resourceManager;

}

