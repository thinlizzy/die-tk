#include <die-tk.h>
#include <convert.h>

using namespace tk;

int main()
{
	Application app;

	Window window(WindowParams("test image load and transparency").dims(500,400));

	auto boss = convertImage(img::Image("boss.png"));
	auto diego = convertImage(img::Image("DIEGO1.jpg"));
	auto explosion = convertImage(img::Image("explosion0.png"));
	auto explosion3 = convertImage(img::Image("explosion3.png"));
	auto bugLeft = convertImage(img::Image("bugLeft.png"));
	auto gameTitle = convertImage(img::Image("gameTitle.png"));
	window.onPaint([&](Canvas & canvas, Rect) {
		diego->drawInto(canvas, Point(0,0));
		explosion->drawInto(canvas, Point(5,5));
		explosion3->drawInto(canvas, Point(80,5));
		boss->drawInto(canvas, Point(20,100));
		bugLeft->drawInto(canvas, Point(300,5));
		gameTitle->drawInto(canvas, Point(200,40));
	});

	// TODO add transparency to PaintBox and to Image
	PaintBox p(window, ControlParams().start(5,40).dims(32,32).backgroundColor(RGBColor(120,0,0)));
	p.onPaint([&](Canvas & canvas, Rect) {
		explosion->drawInto(canvas, Point());
	});
	Image i(window, ControlParams().start(p.pos().addY(32)).dims(p.dims()));
	i.setImage(explosion);

	do {
		app.waitForMessages();
		app.processMessages();
	} while( window.visible() );
}
