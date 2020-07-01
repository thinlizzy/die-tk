#ifndef DIE_TK_CUSTOMCONTROLIMPL_2020_29_06_H
#define DIE_TK_CUSTOMCONTROLIMPL_2020_29_06_H

#include "../ControlParams.h"
#include "../objects/Rect.h"
#include "../Canvas.h"

namespace tk {

class CustomControlImpl {
public:
	Rect rect;
	bool visible;

	CustomControlImpl(ControlParams const & controlParams);

	virtual void draw(Canvas & canvas, Rect rect) const = 0;
};

}

#endif
