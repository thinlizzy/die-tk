#ifndef PAINTBOX_WIN32_H_DIE_TK_2016_08
#define PAINTBOX_WIN32_H_DIE_TK_2016_08

#include "../NativeControlWin32.h"

namespace tk {

class PaintBoxImpl: public NativeControlImpl {
public:
	PaintBoxImpl(HWND parentHwnd, ControlParams const & params);

	virtual PaintBoxImpl * clone() const;
};

}

#endif
