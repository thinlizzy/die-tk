#ifndef RESOURCEMANAGER_H_DIE_TK_LINUX_DIEGO_2014_SEP_01
#define RESOURCEMANAGER_H_DIE_TK_LINUX_DIEGO_2014_SEP_01

#include <memory>
#include <unordered_map>
#include "WindowImplX11.h"

#include <X11/Xlib.h>

namespace tk {

class ResourceManager {
	typedef std::unordered_map<::Window,std::weak_ptr<WindowImpl>> WindowMap;
	WindowMap windowMap;
public:
	Display * dpy;
	ResourceManager();
	~ResourceManager();

	void processMessages();
	void registerWindow(std::shared_ptr<WindowImpl> window);
	std::shared_ptr<WindowImpl> findWindow(::Window windowId);
};

extern ResourceManager resourceManager;

}

#endif /* RESOURCEMANAGER_H_ */
