#include "die-tk.h"
#include "die-tk-image/convert.h"
#include <iostream>
#include <string>
using namespace std::literals::string_literals;

namespace {

#ifdef IMAGE_DIR

#define xstr(a) str(a)
#define str(a) #a

std::string imageDir = xstr(IMAGE_DIR);

#else

#ifdef BAZEL_FOR_WINDOWS
std::string baseDir = "../../../../../../"s;
#else
std::string baseDir = "";
#endif

std::string imageDir = baseDir + "examples/ImageScaling/"s;

#endif

}

using namespace tk;

int main() {
	Application app;

	Window window(WindowParams("test image scaling with transparency!").dims({200,200}));

	auto explosion = convertImage(img::Image(imageDir + "explosion0.png"));
	auto bug = convertImage(img::Image(imageDir + "bugLeft.png"));
	window.onPaint([&](Canvas & canvas, Rect rect) {
		canvas.fillRect(rect,RGBColor(200,32,20));
		canvas.drawImage(explosion,Point(5,5));
		canvas.drawImage(explosion,Rect::closed(Point(30,5),WDims(32,64)));
		canvas.drawImage(bug,Point(5,100));
		canvas.copyRectImage(
			bug,
			Rect::closed(Point(5,5),WDims(20,12)),
			Point(100,100));
	});

	do {
		app.waitForMessages();
		app.processMessages();
	} while( window.visible() );
}
