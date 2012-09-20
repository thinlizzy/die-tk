#ifndef WINDOW_PARAMS_H_gfu43983tgfff45kh03hjg
#define WINDOW_PARAMS_H_gfu43983tgfff45kh03hjg

#include "WindowObjects.h"
#include <limits>
#include <string>

namespace tk {

class WindowParams {
public:
	std::string text_;
	Point start_;
	WDims dims_;
	int initialState;

	static inline Point defaultPos() { int min = std::numeric_limits<int>::min(); return Point(min,min); }
	static inline WDims defaultDims() { int min = std::numeric_limits<int>::min(); return WDims(min,min); }

	WindowParams():
		text_("unnamed window"),
		start_(defaultPos()),
		dims_(defaultDims()),
		initialState(ws_visible)
	{
	}

	WindowParams & text(std::string const & value) { text_ = value; return *this; }
	WindowParams & start(Point value) { start_ = value; return *this; }
	WindowParams & start(int x, int y) { start_ = Point(x,y); return *this; }
	WindowParams & dims(WDims value) { dims_ = value; return *this; }
	WindowParams & dims(int width, int height) { dims_ = WDims(width,height); return *this; }

	WindowParams & addState(WindowState value) { initialState |= value; return *this; }
	WindowParams & removeState(WindowState value) { initialState &= ~value; return *this; }

	bool isDefaultPos() const { return start_ == WindowParams::defaultPos(); }
	bool isDefaultDims() const { return dims_ == WindowParams::defaultDims(); }
};

}

#endif
