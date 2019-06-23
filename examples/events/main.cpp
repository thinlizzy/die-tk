#include <iostream>
#include "die-tk.h"
#include "model/TextLog.h"
#include "model/Ball.h"
#include "model/Painting.h"
#include "view/LinesView.h"
#include "view/BallView.h"
#include "view/PaintingView.h"

using namespace std;
using namespace tk;

// the main function acts as a controller
int main() {
	// -----------------the user can type at the first window---------------------------------------------- //
	
	// horizontal bounds are not being checked for simplicity and it is left as an exercise for the reader :)
	
	auto logWindow = Window(WindowParams("log").dims({320,200}));

	auto log = TextLog();
	auto linesView = LinesView(log, logWindow);

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
		return newDims.fitInto(WDims(800, 600));
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

	// -----------------the user can control a ball at the second window with the arrow keys----------------------------- //

	// a "smoother play" would need manual sync during the message loop.
	// an alternative would be to post user events as they were sync messages in order to have a continuous movement.
	// this is left as an exercise for the reader :)

	auto gameWindow = Window(WindowParams("game").dims({200, 200}).start({300, 300}));
	auto ball = Ball();
	ball.setLimits(gameWindow.dims());
	auto ballView = BallView(ball, gameWindow);

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

	// update ball limits and drag ball when resizing over it
	gameWindow.afterResize([&](WDims newDims) {
		ball.setLimits(newDims);

		auto pos = ball.rect().topLeft();
		if( newDims.width <= ball.rect().right ) {
			ball.setRight(newDims.width - 1);
		}
		if( newDims.height <= ball.rect().bottom ) {
			ball.setBottom(newDims.height - 1);
		}
		if( pos != ball.rect().topLeft()) {
			ballView.moveBall(ball.rect().topLeft() - pos);
		}
	});

	// repaint = redraw ball if touched by rect
	gameWindow.onPaint([&](Canvas & canvas, Rect rect) {
		ballView.drawIfInRect(rect);
	});

	// -----------------the user can paint with the mouse at the third window ----------------------------- //

	auto painterWindow = Window(WindowParams("paint").dims({200, 200}).start({50, 300}));
	auto painting = Painting(painterWindow.dims());
	auto paintingView = PaintingView(painting,painterWindow);

	painterWindow.onMouseEnter([&](Point point) {
		painting.setCrosshair(point);
		paintingView.drawCrosshair(point);
	});

	painterWindow.onMouseOver([&](Point point) {
		auto pos = painting.posCrosshair();
		painting.setCrosshair(point);
		paintingView.move(pos, point);
	});

	painterWindow.onMouseDown([&](MouseEvent evt, Point pt) {
		if( evt.button == MouseButton::left ) {
			painting.penDown();
			paintingView.updatePen();
		}
	});

	painterWindow.onMouseUp([&](MouseEvent evt, Point pt) {
		if( evt.button == MouseButton::left ) {
			painting.penUp();
			paintingView.updatePen();
		}
	});

	// awful implementation just for simplicity, since onMouseLeave can happen before onMouseUp if the mouse button it is still pressed
	// it would be great exiting and entering the window w/o losing the pen down
	painterWindow.onMouseLeave([&](Point point) {
		auto pos = painting.posCrosshair();
		painting.penUp();
		paintingView.eraseCrosshair(pos);
	});

	// onPaint = redraw correct rect from imagebuf
	painterWindow.onPaint([&](Canvas & canvas, Rect rect) {
		paintingView.redraw(canvas, rect);
	});

	// TODO image canvas resizing is a little buggy on X11, leaving garbage in the new image
	// resize canvas if any dimension is greater to the previous one
	// redraw new rects if window is bigger
	auto oldPainterDims = painterWindow.dims();
	painterWindow.afterResize([&](WDims newDims) {
		painting.resize(newDims);
		if( newDims.width > oldPainterDims.width ) {
			auto rect = painterWindow.rect();
			rect.left = oldPainterDims.width;
			paintingView.redraw(painterWindow.canvas(), rect);
		}
		if( newDims.height > oldPainterDims.height ) {
			auto rect = painterWindow.rect();
			rect.top = oldPainterDims.height;
			paintingView.redraw(painterWindow.canvas(), rect);
		}
		oldPainterDims = newDims;
	});

	// exit the app when any of its windows is closed
	auto open = true;
	logWindow.onClose([&open]() -> bool {
		open = false;
		return true;
	});
	gameWindow.onClose([&open]() -> bool {
		open = false;
		return true;
	});
	painterWindow.onClose([&open]() -> bool {
		open = false;
		return true;
	});

	// simple message loop that most applications will use

	Application app;
	do {
		app.waitForMessages();
		app.processMessages();
	} while( open );
}
