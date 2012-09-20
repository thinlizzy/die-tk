#ifndef WINDOW_IMPL_WIN_32_H_kd93wru34924j32f
#define WINDOW_IMPL_WIN_32_H_kd93wru34924j32f

#include <windows.h>
#include <memory>
#include <unordered_map>

#include "../Window.h"
#include "../WindowParams.h"
#include "CanvasImplWin32.h"
#include "controls/NativeControlWin32.h"

#pragma warning( push )
#pragma warning( disable : 4250 )

namespace tk {

class WindowClass {
public:
	WNDCLASS wc;

	WindowClass();
};

class WindowImpl: public NativeControlImpl, public Window {
	static WindowClass windowClass;

	typedef std::unordered_map<HWND,std::shared_ptr<NativeControlImpl>> Controls;
	Controls controls;

	int state_;

	WindowImpl(WindowImpl const &); // prevent copying

	void createWindow(Point pos, WDims dims, char const windowname[], char const classname[], DWORD style);
public:
	WindowImpl(WindowParams const & params);

	virtual ~WindowImpl();

	virtual void setDims(WDims dims);
	virtual void setRect(Rect rect);

	virtual int state() const;

	virtual void show();
	virtual void hide();

	void clear(RGBColor const & color);

	virtual bool processMessage(UINT message, WPARAM wParam, LPARAM lParam);

	RECT windowRect() const;

	std::shared_ptr<NativeControlImpl> findControl(HWND handle);
	void registerControl(std::shared_ptr<NativeControlImpl> control);
};


}

#pragma warning( pop )

#endif

