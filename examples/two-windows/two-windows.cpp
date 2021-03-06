#include "die-tk.h"

using namespace std;
using namespace tk;

WDims getDims(bool big) { return big ? WDims(200,100) : WDims(100,200); }

int main() {
	Application app;

	auto firstWindowWide = true;
	auto swapWindowSizes = [&firstWindowWide](Window & w1, Window & w2) {
		w2.setDims(getDims(firstWindowWide));
		firstWindowWide = ! firstWindowWide;
		w1.setDims(getDims(firstWindowWide));
	};

	auto window = Window(WindowParams("first").start({10,10}).dims({200,100}));
	auto window2 = Window(WindowParams("second").start({250,10}).dims({100,200}).addState(ws_noresize));
	window.onMouseDown([&](MouseEvent e, Point pt) {
		switch(e.button) {
			case MouseButton::left: {
				swapWindowSizes(window,window2);
			} break;
			case MouseButton::right:
				swapWindowSizes(window,window2);
				break;
		}
	});
	window2.onMouseDown([&](MouseEvent e, Point pt) {
		switch(e.button) {
			case MouseButton::left:
				swapWindowSizes(window,window2);
				break;
			case MouseButton::right:
				swapWindowSizes(window,window2);
				break;
		}
	});
	window.onPaint([](Canvas & canvas, Rect rect) {
		Points points = { {40,10}, {80,40}, {40,80}, {10,40}, };
		canvas.drawPoly(points,RGBColor(0,180,0));
	});
	window2.onPaint([](Canvas & canvas, Rect rect) {
		canvas.roundRect(Rect::closed(Point(10,10),WDims(70,80)),WDims(10,20),RGBColor(180,180,0),RGBColor(180,180,0));
	});

	auto open = true;
	auto closeFn = [&open]() -> bool { open = false; return true; };
	window.onClose(closeFn);
	window2.onClose(closeFn);

	window.onResize([](WDims d) -> WDims {
		if( d.width > 200 ) d.width = 200;
		if( d.height > 400 ) d.height = 400;
		return d;
	});

	do {
		app.waitForMessages();
		app.processMessages();
	} while( open );
}
