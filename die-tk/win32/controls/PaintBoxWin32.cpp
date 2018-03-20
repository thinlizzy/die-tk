#include "PaintBoxWin32.h"
#include "../ResourceManager.h"

namespace tk {

class PaintBoxClass {
public:
	WNDCLASSW wc;

	PaintBoxClass()
	{
		// create window class
		wc.style = CS_PARENTDC;
		wc.lpfnWndProc = WndProc;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = GetModuleHandle(NULL);
		wc.hIcon = NULL;
		wc.hCursor = LoadCursor(NULL,IDC_ARROW);
		wc.lpszMenuName = NULL;
		wc.lpszClassName = L"die-tk-paintbox";
		RegisterClassW(&wc);
	}
};

PaintBoxClass paintBoxClass;

PaintBoxImpl::PaintBoxImpl(HWND parentHwnd, ControlParams const & params):
	NativeControlImpl(parentHwnd,params,paintBoxClass.wc.lpszClassName,0)
{
}

PaintBoxImpl * PaintBoxImpl::clone() const
{
	return new PaintBoxImpl(getParentHandle(),getControlData());
}

bool PaintBoxImpl::transparent() const
{
	return ! static_cast<bool>(backgroundBrush); // it achieves transparency by not having a background
}

void PaintBoxImpl::setTransparentBackground()
{
	backgroundBrush.reset(0);
}

void PaintBoxImpl::repaint()
{
	RECT rc = getClientRect();
	InvalidateRect(GetParent(hWnd),&rc,true); // needs to be true in order to support transparency
}

}
