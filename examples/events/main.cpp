#include <die-tk.h>
#include <iostream>
#include "model/TextLog.h"
#include "model/Ball.h"
#include "view/LinesView.h"
#include "view/BallView.h"

using namespace std;
using namespace tk;

// the main function acts as a controller
int main()
{
	Window logWindow{WindowParams("log").dims(320,200)};

	TextLog log;
	LinesView linesView{log,logWindow};

	// key press = write stuff on the screen, like it were a log
	logWindow.onKeypress([&](char key) {
		auto status = log.processChar(key);
		switch(status) {
		case TextLog::Status::newChar:
			linesView.drawCharLastLine(key);
			break;
		case TextLog::Status::deleteChar:
			linesView.removeCharLastLine(key);
			break;
		case TextLog::Status::newLine:
			linesView.verifyScroll();
			break;
		}
		return '\0';
	});

	// resize = limit to 800,600
	logWindow.onResize([](WDims newDims) {
		return newDims.fitInto(WDims(800,600));
	});
	// redraw lines if the height is bigger, scroll if the height is smaller
	auto lastHeight = logWindow.dims().height;
	logWindow.afterResize([&](WDims newDims) {
		if( newDims.height != lastHeight ) {
			lastHeight = newDims.height;
			linesView.verifyScroll();
		}
	});

	// repaint = redraw lines according to the intersecting rectangle
	logWindow.onPaint([&](Canvas & canvas, Rect rect) {
		linesView.updateLinesInRect(rect);
	});

	// horizontal bounds are not being checked for simplicity and it is left as an exercise for the reader :)

// ----------------------------------------------------------------------------- //

	Window gameWindow{WindowParams("game").dims(200,200).start(300,300)};
	Ball ball;
	ball.setLimits(gameWindow.dims());
	BallView ballView{ball,gameWindow};

	// key down = start moving the ball in a direction
	gameWindow.onKeyDown([&](WindowKey key) {
		auto result = ball.processKey(key);
		switch(result.status) {
		case Ball::Status::shift:
			ballView.moveBall(result.offset);
			break;
		}
		return k_NONE;
	});

	// key up = stop moving the ball in a direction
	gameWindow.onKeyUp([&](WindowKey key) {
		auto result = ball.processKeyRelease(key);
		switch(result.status) {
		case Ball::Status::shift:
			ballView.moveBall(result.offset);
			break;
		}
		return k_NONE;
	});

	// a smoother "play" needs manual sync or to post sync messages to have a continuous movement which is left as an exercise for the reader :)

	// update ball limits and drag ball when resizing over it
	gameWindow.afterResize([&](WDims newDims) {
		ball.setLimits(newDims);

		auto pos = ball.rect().topLeft();
		if( newDims.width <= ball.rect().right ) {
			ball.setRight(newDims.width-1);
		}
		if( newDims.height <= ball.rect().bottom ) {
			ball.setBottom(newDims.height-1);
		}
		if( pos != ball.rect().topLeft() ) {
			ballView.moveBall(ball.rect().topLeft() - pos);
		}
	});

	// repaint = redraw ball if touched by rect
	gameWindow.onPaint([&](Canvas & canvas, Rect rect) {
		ballView.drawIfInRect(rect);
	});

/*
	window.onMouseDown();
	window.onMouseUp();

	window.onMouseEnter();
	window.onMouseOver();
	window.onMouseLeave();
*/

	// exit the app when any of its windows is closed
	bool open = true;
	logWindow.onClose([&open]() -> bool { open = false; return true; });
	gameWindow.onClose([&open]() -> bool { open = false; return true; });

	Application app;
	do {
		app.waitForMessages();
		app.processMessages();
	} while( open );
}

