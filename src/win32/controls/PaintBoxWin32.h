#ifndef PAINTBOX_WIN32_H_j3422224fdsfdsfs342432432332
#define PAINTBOX_WIN32_H_j3422224fdsfdsfs342432432332

#include "NativeControlWin32.h"
#include "../../controls/PaintBox.h"

namespace tk {

class PaintBoxImpl: public NativeControlImpl, public PaintBox {
public:
	PaintBoxImpl(HWND parent_hWnd, ControlParams const & params);
};

}

#endif
