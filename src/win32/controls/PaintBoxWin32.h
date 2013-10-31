#ifndef PAINTBOX_WIN32_H_j3422224fdsfdsfs342432432332
#define PAINTBOX_WIN32_H_j3422224fdsfdsfs342432432332

#include "NativeControlWin32.h"

namespace tk {

class PaintBoxImpl: public NativeControlImpl {
public:
	PaintBoxImpl(HWND parentHwnd, ControlParams const & params);
    
    virtual PaintBoxImpl * clone() const;
};

}

#endif
