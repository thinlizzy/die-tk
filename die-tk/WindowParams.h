#ifndef WINDOW_PARAMS_H_gfu43983tgfff45kh03hjg
#define WINDOW_PARAMS_H_gfu43983tgfff45kh03hjg

#include <limits>
#include "NativeString.h"
#include "WindowObjects.h"
#include "objects/Color.h"
#include "objects/Point.h"
#include "objects/Dimension.h"
#include "util/optional.h"

namespace tk {

class WindowParams {
public:
	NativeString text_;
	Point start_;
	WDims dims_;
    optional<RGBColor> backgroundColor_;
	int initialState;

	static inline Point defaultPos() { int min = std::numeric_limits<int>::min(); return Point(min,min); }
	static inline WDims defaultDims() { int min = std::numeric_limits<int>::min(); return WDims(min,min); }

	explicit WindowParams(NativeString text_ = NativeString()):
		text_(std::move(text_)),
		start_(defaultPos()),
		dims_(defaultDims()),
		initialState(ws_visible)
	{}

	WindowParams & text(NativeString const & value) { text_ = value; return *this; }
	WindowParams & start(Point value) { start_ = value; return *this; }
	WindowParams & dims(WDims value) { dims_ = value; return *this; }
	WindowParams & backgroundColor(RGBColor value) { backgroundColor_ = value; return *this; }

	WindowParams & addState(WindowState value) { initialState |= value; return *this; }
	WindowParams & removeState(WindowState value) { initialState &= ~value; return *this; }
	WindowParams & states(int value) { initialState = value; return *this; }

	bool isDefaultPos() const { return start_ == WindowParams::defaultPos(); }
	bool isDefaultDims() const { return dims_ == WindowParams::defaultDims(); }
};

}

#endif
