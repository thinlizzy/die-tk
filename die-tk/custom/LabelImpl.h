#ifndef DIE_TK_CUSTOM_LABELIMPL_2020_29_06_H
#define DIE_TK_CUSTOM_LABELIMPL_2020_29_06_H

#include "CustomControlImpl.h"

namespace tk {

class LabelImpl: public CustomControlImpl {
public:
	NativeString text;
	RGBColor color;
	optional<RGBColor> backgroundColor;
	bool autosize;

	LabelImpl(ControlParams const & controlParams);

	void draw(Canvas & canvas, Rect rect) const override;
};

}

#endif
