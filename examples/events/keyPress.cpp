#include <die-tk.h>
#include <iostream>
#include "model/TextLog.h"
#include "view/LinesView.h"

using namespace std;
using namespace tk;

// the main function acts as a controller
int main()
{
	Window window(WindowParams("testex").dims(320,200));

	bool open = true;
	window.onClose([&open]() -> bool { open = false; return true; });

	TextLog log;
	LinesView linesView(log,window);

	// key press = write stuff on the screen, like it were a log
	window.onKeypress([&](char key) {
		auto status = log.processChar(key);
		switch(status) {
		case Status::newChar:
			linesView.drawCharLastLine(key);
			break;
		case Status::deleteChar:
			linesView.removeCharLastLine(key);
			break;
		case Status::newLine:
			linesView.verifyScroll();
			break;
		}
		return '\0';
	});

	// resize = limit to 800,600
	window.onResize([](WDims newDims) {
		return newDims.fitInto(WDims(800,600));
	});
	// redraw lines if the height is bigger, scroll if the height is smaller
	auto lastHeight = window.dims().height;
	window.afterResize([&](WDims newDims) {
		if( newDims.height != lastHeight ) {
			lastHeight = newDims.height;
			linesView.verifyScroll();
		}
	});

	// repaint = redraw lines according to the intersecting rectangle
	window.onPaint([&](Canvas & canvas, Rect rect) {
		linesView.updateLinesInRect(rect);
	});

	// horizontal bounds are not being checked for simplicity and it is left as an exercise for the reader :)

/*
	window.onKeyDown();
	window.onKeyUp();

	window.onMouseDown();
	window.onMouseUp();

	window.onMouseEnter();
	window.onMouseOver();
	window.onMouseLeave();
*/

	Application app;
	do {
		app.waitForMessages();
		app.processMessages();
	} while( open );
}

