#ifndef CONTROL_PARAMS_H_gfu43983tgfff45kh03hjg312
#define CONTROL_PARAMS_H_gfu43983tgfff45kh03hjg312

#include <iosfwd>
#include "NativeString.h"
#include "WindowObjects.h"
#include "objects/Color.h"
#include "objects/Dimension.h"
#include "objects/Point.h"
#include "util/optional.h"

namespace tk {

class ControlParams {
public:
	NativeString text_;
	Point start_;
	WDims dims_;
    bool visible_ = true;
	Scrollbar scrollbar_ = Scrollbar::none;
	bool autosize_ = false;
	RGBColor color_;
    optional<RGBColor> backgroundColor_;
    Cursor cursor_ = Cursor::defaultCursor;

	ControlParams & text(NativeString const & value) { text_ = value; return *this; }
	ControlParams & start(Point value) { start_ = value; return *this; }
	ControlParams & dims(WDims value) { dims_ = value; return *this; }
	ControlParams & visible(bool value = true) { visible_ = value; return *this; }
	ControlParams & scrollbar(Scrollbar value) { scrollbar_ = value; return *this; }
	ControlParams & autosize(bool value = true) { autosize_ = value; return *this; }
	ControlParams & color(RGBColor value) { color_ = value; return *this; }
	ControlParams & backgroundColor(RGBColor value) { backgroundColor_ = value; return *this; }
	ControlParams & backgroundColor(optional<RGBColor> value) { backgroundColor_ = value; return *this; }
	ControlParams & cursor(Cursor value) { cursor_ = value; return *this; }
};

std::ostream & operator<<(std::ostream & os, ControlParams const & cp);

}

#endif
