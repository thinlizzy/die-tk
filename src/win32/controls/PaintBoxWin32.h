#ifndef PAINTBOX_WIN32_H_j3422224fdsfdsfs342432432332
#define PAINTBOX_WIN32_H_j3422224fdsfdsfs342432432332

#include "NativeControlWin32.h"

// TODO overlapping other controls with a PaintBox might cause them to not respond, unless the paint box is back on Z-order

namespace tk {

class PaintBoxImpl: public NativeControlImpl {
public:
	PaintBoxImpl(HWND parent_hWnd, ControlParams const & params);
};

}

#endif
