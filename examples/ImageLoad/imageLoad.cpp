#include "die-tk.h"
#include "die-tk-image/convert.h"
#include <iostream>
#include <string>
using namespace std::literals::string_literals;

namespace {

#ifdef BAZEL_FOR_WINDOWS
std::string baseDir = "../../../../../../"s;
#else
std::string baseDir = "";
#endif

// from data dir dependency. see how to relocate it
std::string imageDir = baseDir + "examples/imageLoad/"s;

}

using namespace tk;

int main()
{
	Application app;

	Window window(WindowParams("test image load and transparency!").dims({700,400}));

	auto boss = convertImage(img::Image(imageDir + "boss.png"));
	auto diego = convertImage(img::Image(imageDir + "DIEGO1.jpg"));
	auto explosion = convertImage(img::Image(imageDir + "explosion0.png"));
	auto explosion3 = convertImage(img::Image(imageDir + "explosion3.png"));
	auto bugLeft = convertImage(img::Image(imageDir + "bugLeft.png"));
	auto gameTitle = convertImage(img::Image(imageDir + "gameTitle.png"));
	window.onPaint([&](Canvas & canvas, Rect) {
		diego->drawInto(canvas,Point(0,0));
		explosion->drawInto(canvas,Point(5,5));
		explosion3->drawInto(canvas,Point(80,5));
		boss->drawInto(canvas,Point(20,100));
		bugLeft->drawInto(canvas,Point(300,15));
		gameTitle->drawInto(canvas,Point(300,40));
	});

	PaintBox p(window, ControlParams().start({5,40}).dims({40,40}));
	p.onPaint([&](Canvas & canvas, Rect rect) {
		std::cout << rect << std::endl;
		canvas.fillRect(Rect::open(Point(2,2),WDims(6,6)),RGBColor(100,0,0));
		explosion->drawInto(canvas,Point(0,0));
		// TODO X11 copy rect into fails with smaller rectangles
		// TODO X11 PaintBox transparency fails
		//explosion->copyRectInto(canvas,rect,rect.topLeft());
	});
	std::cout << p.rect() << std::endl;

	// TODO remove this ifdef after implementing image in X11
#ifdef WIN32
	// TODO make Image support transparent images - it is cloneBitmap()'s fault
	Image i(window, ControlParams().start(p.pos().addY(32)).autosize());
	i.setImage(explosion);
	Image j = i.clone();
	j.setBackground(RGBColor(120,0,0));
	j.setPos(i.pos().addX(32));
	j.setDims(WDims(40,40));
#endif

	do {
		app.waitForMessages();
		app.processMessages();
	} while( window.visible() );
}
