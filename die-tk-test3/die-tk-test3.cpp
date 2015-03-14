#include <die-tk.h>
#include <die-tk-image/convert.h>

using namespace tk;

int main()
{
	Application app;

	Window window(WindowParams("test image load").dims(400,400));

	image::Ptr boss = convertImage(img::Image("boss.png"));
	image::Ptr diego = convertImage(img::Image("DIEGO1.jpg"));
	window.onPaint([&](Canvas & canvas, Rect) {
		boss->drawInto(canvas, Point(10,10));
		diego->drawInto(canvas, Point(100,100));
	});

	do {
		app.waitForMessages();
		app.processMessages();
	} while( window.visible() );
}
