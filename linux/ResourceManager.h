#ifndef RESOURCEMANAGER_H_DIE_TK_LINUX_DIEGO_2014_SEP_01
#define RESOURCEMANAGER_H_DIE_TK_LINUX_DIEGO_2014_SEP_01

#include <memory>
#include <unordered_map>
#include <X11/Xlib.h>
#include "NativeControlImpl.h"
#include "WindowImplX11.h"
#include "../src/util/singleton.h"

namespace tk {

class ResourceManager {
	using WindowedControlMap = std::unordered_map<::Window,std::weak_ptr<NativeControlImpl>>;
	using WindowMap = std::unordered_map<::Window,std::weak_ptr<WindowImpl>>;
	WindowedControlMap controlMap;
	WindowMap windowMap;
public:
	Display * dpy;
	ResourceManager();
	~ResourceManager();

	void processMessages();
	void registerControl(std::shared_ptr<NativeControlImpl> const & control);
	void unregisterControl(std::shared_ptr<NativeControlImpl> const & control);
	void registerClonedControl(std::shared_ptr<NativeControlImpl> const & control);
	void registerWindow(std::shared_ptr<WindowImpl> const & window);
	void unregisterWindow(std::shared_ptr<WindowImpl> const & window);
	std::shared_ptr<WindowImpl> findWindow(::Window windowId);

	// the screen window
	::Window root();

	// it also works with depth 32, which enables transparency on composite WMs.
	// the problem is children windows become transparent in relation to the desktop as well
	// (literally they open a HOLE in the app)
	constexpr static int defaultDepth = 24;

	char const * getAtomName(Atom atom);
	::Window createTopLevelWindow(int x, int y, int width, int height);
	::Window createWindow(int x, int y, int width, int height, ::Window parentId, int depth = defaultDepth);
private:
	std::shared_ptr<NativeControlImpl> findWindowedControl(::Window windowId);
};

using ResourceManagerSingleton = singleton<ResourceManager>;

}

#endif /* RESOURCEMANAGER_H_ */
