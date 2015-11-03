#ifndef RESOURCEMANAGER_H_DIE_TK_LINUX_DIEGO_2014_SEP_01
#define RESOURCEMANAGER_H_DIE_TK_LINUX_DIEGO_2014_SEP_01

#include <memory>
#include <unordered_map>
#include <X11/Xlib.h>
#include "WindowImplX11.h"

namespace tk {

class ResourceManager {
	typedef std::unordered_map<::Window,std::weak_ptr<WindowImpl>> WindowMap;
	WindowMap windowMap;
	// no control map here. only win32 version is registering controls for now
public:
	Display * dpy;
	ResourceManager();
	~ResourceManager();

	void processMessages();
	void registerWindow(std::shared_ptr<WindowImpl> const & window);
	void unregisterWindow(std::shared_ptr<WindowImpl> const & window);
	std::shared_ptr<WindowImpl> findWindow(::Window windowId);

	// the screen window
	::Window root();

	char const * getAtomName(Atom atom);
};

extern ResourceManager resourceManager;

}

#endif /* RESOURCEMANAGER_H_ */
