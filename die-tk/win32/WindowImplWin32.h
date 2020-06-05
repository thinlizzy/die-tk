#ifndef WINDOW_IMPL_WIN_32_H_kd93wru34924j32f
#define WINDOW_IMPL_WIN_32_H_kd93wru34924j32f

#include <windows.h>
#include <memory>
#include <unordered_map>
#include <unordered_set>

#include "../WindowParams.h"
#include "../util/optional.h"
#include "CanvasImplWin32.h"
#include "NativeControlWin32.h"
#include "components/WindowComponent.h"

namespace tk {

class WindowClass {
public:
	WNDCLASSW wc;

	WindowClass();
};

class WindowImpl: public NativeControlImpl {
	static WindowClass windowClass;

	typedef std::unordered_map<HWND,std::shared_ptr<NativeControlImpl>> Controls;
	Controls controls;

	int state_;

	WindowImpl(WindowImpl const &); // prevent copying

	static HWND createWindow(Point pos, WDims dims, wchar_t const windowname[], wchar_t const classname[], DWORD style);    
public:
    typedef std::unordered_set<std::shared_ptr<WindowComponent>> Components;
    Components components;
    
	WindowImpl(WindowParams const & params);

	virtual ~WindowImpl();
    
    virtual WindowImpl * clone() const;

	virtual void setDims(WDims dims);
	virtual void setRect(Rect rect);

	int state() const;
    
    void setBorders(bool value);
    
	virtual void show();
	virtual void hide();

	virtual optional<LRESULT> processMessage(UINT message, WPARAM & wParam, LPARAM & lParam);

	std::shared_ptr<NativeControlImpl> findControl(HWND handle);
	void registerControl(std::shared_ptr<NativeControlImpl> control);
    void unregisterControl(std::shared_ptr<NativeControlImpl> control);

	AllowOperation onClose(AllowOperation callback);
	ProcessResize onResize(ProcessResize callback);
	HandleResize afterResize(HandleResize callback);

	HandleEvent onUserEvent(HandleEvent callback);
private:
    WDims getWindowDims(WDims dims);
};

}

#endif

