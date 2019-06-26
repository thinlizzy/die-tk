#include "die-tk.h"

using namespace tk;

int main() {
	Application app;

	auto window = Window(WindowParams("ImageCanvas example").start({10,10}).dims({300,200}));

	auto imageCanvas = ImageCanvas::create({20,20});
	imageCanvas->fillRect(Rect::open({},{20,20}),RGBColor(200,0,0));
	imageCanvas->fillRect(Rect::open({5,5},{10,10}),RGBColor(0,200,0));
	auto image = imageCanvas->finishAndCreateImage();

	window.onPaint([&image](Canvas & canvas, Rect rect) {
		image->drawInto(canvas,Point(30,30));
		image->drawInto(canvas,Point(30,60));
		image->drawInto(canvas,Point(60,30));
		image->drawInto(canvas,Point(60,60));
	});

	do {
		app.waitForMessages();
		app.processMessages();
	} while( window.visible() );
}
