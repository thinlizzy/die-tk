#include <die-tk.h>
#include <iostream>
#include <algorithm>
using namespace tk;

int main()
{
	auto window = Window{WindowParams{"paintboxes"}.dims({200,400}).backgroundColor({100,0,0})};
	auto bkColor = RGBColor{150,150,40};
	auto pb1 = PaintBox{window,ControlParams{}.start({10,10}).dims({100,100}).backgroundColor(bkColor)};
	auto color1 = RGBColor{10,10,200};
	auto pb2 = PaintBox{window,ControlParams{}.start({pb1.x(),150}).dims(pb1.dims())};
	auto color2 = RGBColor{200,10,200};

	auto painter = [](RGBColor & color) {
		return [&color](Canvas & canvas, Rect rect) {
			// TODO intersect with rect
			canvas.fillRect(Rect::open(Point{30,30},WDims{40,40}),color);
		};
	};
	pb1.onPaint(painter(color1));
	pb2.onPaint(painter(color2));

	auto clicker = [&](MouseEvent e,Point) {
		std::swap(color1,color2);
		pb1.repaint();
		pb2.repaint();
	};
	pb1.onMouseDown(clicker);
	pb2.onMouseDown(clicker);

	Application app;
	do {
		app.waitForMessages();
		app.processMessages();
	} while( window.visible() );
}
