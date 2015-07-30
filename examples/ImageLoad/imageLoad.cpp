#include <die-tk.h>
#include <convert.h>

using namespace tk;

int main()
{
	Application app;

	Window window(WindowParams("test image load and transparency").dims(400,400));

	image::Ptr boss = convertImage(img::Image("boss.png"));
	image::Ptr diego = convertImage(img::Image("DIEGO1.jpg"));
	image::Ptr explosion = convertImage(img::Image("explosion0.png"));
	image::Ptr explosion3 = convertImage(img::Image("explosion3.png"));
	window.onPaint([&](Canvas & canvas, Rect) {
		diego->drawInto(canvas, Point(0,0));
		explosion->drawInto(canvas, Point(5,5));
		explosion3->drawInto(canvas, Point(80,5));
		boss->drawInto(canvas, Point(20,100));
	});

	do {
		app.waitForMessages();
		app.processMessages();
	} while( window.visible() );
}
