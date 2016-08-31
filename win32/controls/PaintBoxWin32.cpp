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
		wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
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

}
