#ifndef RESOURCE_MANAGER_WIN32_H_jf458943jfg
#define RESOURCE_MANAGER_WIN32_H_jf458943jfg

#include <memory>
#include <unordered_map>
#include <windows.h>

#include "NativeControlWin32.h"
#include "WindowImplWin32.h"
#include "ScopedObjects.h"
#include "../src/util/singleton.h"

namespace tk {

class ResourceManager {
	typedef std::unordered_map<HWND,std::weak_ptr<WindowImpl>> WindowMap;
	typedef std::unordered_map<HWND,std::shared_ptr<NativeControlImpl>> ControlMap;
	WindowMap windowMap;
	ControlMap controlMap;
    scoped::DC scrdc;

	void handleTopLevelWindowMessages(HWND hWnd, UINT message, WPARAM & wParam, LPARAM & lParam);
	std::shared_ptr<NativeControlImpl> getTopLevelWindow(HWND hWnd);
public:
    ResourceManager();
	void registerWindow(std::shared_ptr<WindowImpl> const & window);
	void unregisterWindow(std::shared_ptr<WindowImpl> const & window);
	void registerControl(std::shared_ptr<NativeControlImpl> const & control);
    void unregisterControl(std::shared_ptr<NativeControlImpl> const & control);
	std::shared_ptr<WindowImpl> findWindow(HWND hWnd);
	std::shared_ptr<NativeControlImpl> findControl(HWND hWnd);
    std::shared_ptr<NativeControlImpl> findControlOrWindow(HWND hWnd);

    void processMessages();

	LRESULT WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

    HDC screenDC();
    
	HCURSOR cursors[int(Cursor::numCursors)];
};

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

using ResourceManagerSingleton = singleton<ResourceManager>;

}

#endif
