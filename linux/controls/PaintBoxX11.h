#ifndef PAINTBOX_X11_H_DIE_TK_2016_09
#define PAINTBOX_X11_H_DIE_TK_2016_09

#include "../NativeControlImpl.h"

namespace tk {

class PaintBoxX11: public NativeControlImpl {
public:
	PaintBoxX11(::Window parent, ControlParams const & params);

	virtual PaintBoxX11 * clone() const;
};

}

#endif
