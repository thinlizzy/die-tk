#include "PaintBoxWin32.h"
#include "../ConvertersWin32.h"

#include "../ApplicationWin32.h"

namespace tk {

class PaintBoxClass {
public:
	WNDCLASS wc;

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
		wc.lpszClassName = "die-tk-paintbox";
		RegisterClass(&wc);
	}
};

PaintBoxClass paintBoxClass;

PaintBoxImpl::PaintBoxImpl(HWND parent_hWnd, ControlParams const & params):
	NativeControlImpl(parent_hWnd,params,paintBoxClass.wc.lpszClassName,0)
{
}

}
