#ifndef RESOURCE_MANAGER_WIN32_H_jf458943jfg
#define RESOURCE_MANAGER_WIN32_H_jf458943jfg

#include <memory>
#include <unordered_map>
#include <windows.h>

#include "controls/NativeControlWin32.h"
#include "WindowImplWin32.h"

namespace tk {

class ResourceManager {
	typedef std::unordered_map<HWND,std::shared_ptr<WindowImpl>> WindowMap;
	typedef std::unordered_map<HWND,std::shared_ptr<NativeControlImpl>> ControlMap;
	WindowMap windowMap;
	ControlMap controlMap;

	void handleTopLevelWindowMessages(HWND hWnd, UINT message, WPARAM & wParam, LPARAM & lParam);
	std::shared_ptr<NativeControlImpl> getTopLevelWindow(HWND hWnd);
public:
    ResourceManager();
	void registerWindow(std::shared_ptr<WindowImpl> window);
	void registerControl(std::shared_ptr<NativeControlImpl> control);
    void unregisterControl(std::shared_ptr<NativeControlImpl> control);
	std::shared_ptr<WindowImpl> findWindow(HWND hWnd);
	std::shared_ptr<NativeControlImpl> findControl(HWND hWnd);
    std::shared_ptr<NativeControlImpl> findControlOrWindow(HWND hWnd);

    void processMessages();

	LRESULT WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	HCURSOR cursors[cur_numCursors];
};

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

extern ResourceManager resourceManager;

}

#endif
