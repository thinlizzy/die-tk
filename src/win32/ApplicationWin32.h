#ifndef APPLICATION_WIN32_H_fjj43f4j3f43fj
#define APPLICATION_WIN32_H_fjj43f4j3f43fj

#include <memory>
#include <unordered_map>
#include <windows.h>

#include "../Application.h"

#include "WindowImplWin32.h"
#include "ConvertersWin32.h"

#include "controls/NativeControlWin32.h"
#include "controls/ButtonWin32.h"
#include "controls/PaintBoxWin32.h"
#include "controls/ImageWin32.h"
#include "controls/ComboBoxWin32.h"
#include "controls/EditWin32.h"
#include "controls/MemoWin32.h"
#include "controls/CheckBoxWin32.h"
#include "controls/LabelWin32.h"
#include "controls/TreeViewWin32.h"

#include "components/ImageListWin32.h"

/*

// VS2010 needed this

namespace std {

template<typename T>
struct hash<shared_ptr<T>> {

size_t operator()(shared_ptr<T> const & key) const
{
	return hash<T*>()(key.get());
}

};

}
*/

namespace tk {

class ApplicationImpl {
	typedef std::unordered_map<HWND,std::shared_ptr<WindowImpl>> WindowMap;
	typedef std::unordered_map<HWND,std::shared_ptr<NativeControlImpl>> ControlMap;
	WindowMap windowMap;
	ControlMap controlMap;

	void handleTopLevelWindowMessages(HWND hWnd, UINT message, WPARAM & wParam, LPARAM & lParam);
	std::shared_ptr<NativeControlImpl> getTopLevelWindow(HWND hWnd);
public:
    ApplicationImpl();
	void registerWindow(std::shared_ptr<WindowImpl> window);
	void registerControl(std::shared_ptr<NativeControlImpl> control);
	std::shared_ptr<WindowImpl> findWindow(HWND hWnd);
	std::shared_ptr<NativeControlImpl> findControl(HWND hWnd);

    void processMessages();

	LRESULT WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	bool handleControlCallbacks(UINT message, std::shared_ptr<NativeControlImpl> control, WPARAM & wParam, LPARAM & lParam);
	bool handleWindowCallbacks(UINT message, std::shared_ptr<WindowImpl> window, WPARAM & wParam, LPARAM & lParam);

	std::unordered_map<std::shared_ptr<Control>,HandlePaint> onPaint;
	std::unordered_map<std::shared_ptr<Control>,HandleMouseEvent> onMouse;
	std::unordered_map<std::shared_ptr<Control>,ProcessKeyEvent> onKey;
	std::unordered_map<std::shared_ptr<Control>,ProcessKeypress> onKeypress;
	std::unordered_map<std::shared_ptr<Control>,HandleOperation> onClick;
	std::unordered_map<std::shared_ptr<Control>,HandleOperation> onChange;

	std::unordered_map<WindowPtr,AllowOperation> onClose;
	std::unordered_map<WindowPtr,ProcessResize> onResize;
	std::unordered_map<WindowPtr,HandleEvent> onUserEvent;

	HCURSOR cursors[cur_numCursors];
};

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

extern std::shared_ptr<ApplicationImpl> globalAppImpl;

}

#endif

