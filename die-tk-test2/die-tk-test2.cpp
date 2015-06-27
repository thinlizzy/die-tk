#include <die-tk.h>
#include <iostream>
using namespace std;
using namespace tk;

template<typename T>
void debug(T value) { cout << value << endl; }

template<typename T>
T cycle(T value) {
	auto i = int(value) + 1;
	if( i >= 3 ) i = 0;
	return T(i);
}

int main()
{
	cout << "!!!Hello World!!!" << endl;

	Application app;
	Points points;

    image::Byte rgbbuffer[] = {
        255,255,255, 255,255,255, 255,255,255, 0,255,255, 255,255,255, 255,255,255, 255,255,255,
        255,0,255, 255,255,255, 255,255,255, 0,255,255, 255,255,255, 255,255,255, 255,255,255,
        0,255,255, 255,255,255, 255,255,255, 0,255,255, 255,255,255, 255,255,255, 255,255,255,
        255,255,0, 255,255,255, 255,255,255, 0,255,255, 255,255,255, 255,255,255, 255,255,255,
        0,255,255, 255,255,255, 255,255,255, 255,255,255, 255,255,255, 255,255,255, 255,255,255,
        255,0,255, 255,255,255, 255,255,255, 255,255,255, 255,0,0,   255,0,0,     255,0,0,
        255,255,0, 255,255,255, 255,255,255, 255,255,255, 255,255,255, 255,255,255, 255,255,255,
        255,255,255, 255,255,255, 255,255,255, 255,255,255, 255,255,255, 255,255,255, 255,255,255,
    };
    auto imgRgb = image::create(image::Params(image::Type::RGB,WDims(7,8),rgbbuffer));

    image::Byte rgbbuffer2[] = {
        255,255,255, 255,255,255, 255,255,255, 255,255,255, 255,0,0, 255,0,0, 255,0,0, 255,0,0, 0,255,255, 0,255,255, 0,255,255, 0,255,255,
        255,255,255, 255,255,255, 255,255,255, 255,255,255, 255,0,0, 255,0,0, 255,0,0, 255,0,0, 0,255,255, 0,255,255, 0,255,255, 0,255,255,
        255,255,255, 255,255,255, 255,255,255, 255,255,255, 255,0,0, 255,0,0, 255,0,0, 255,0,0, 0,255,255, 0,255,255, 0,255,255, 0,255,255,
        255,255,255, 255,255,255, 255,255,255, 255,255,255, 255,0,0, 255,0,0, 255,0,0, 255,0,0, 0,255,255, 0,255,255, 0,255,255, 0,255,255,
        255,0,255, 255,0,255, 255,0,255, 255,0,255, 0,255,0, 0,255,0, 0,255,0, 0,255,0, 255,255,0, 255,255,0, 255,255,0, 255,255,0,
        255,0,255, 255,0,255, 255,0,255, 255,0,255, 0,255,0, 0,255,0, 0,255,0, 0,255,0, 255,255,0, 255,255,0, 255,255,0, 255,255,0,
        255,0,255, 255,0,255, 255,0,255, 255,0,255, 0,255,0, 0,255,0, 0,255,0, 0,255,0, 255,255,0, 255,255,0, 255,255,0, 255,255,0,
        255,0,255, 255,0,255, 255,0,255, 255,0,255, 0,255,0, 0,255,0, 0,255,0, 0,255,0, 255,255,0, 255,255,0, 255,255,0, 255,255,0,
        0,0,255, 0,0,255, 0,0,255, 0,0,255, 255,255,255, 255,255,255, 255,255,255, 255,255,255, 255,0,0, 255,0,0, 255,0,0, 255,0,0,
        0,0,255, 0,0,255, 0,0,255, 0,0,255, 255,255,255, 255,255,255, 255,255,255, 255,255,255, 255,0,0, 255,0,0, 255,0,0, 255,0,0,
        0,0,255, 0,0,255, 0,0,255, 0,0,255, 255,255,255, 255,255,255, 255,255,255, 255,255,255, 255,0,0, 255,0,0, 255,0,0, 255,0,0,
        0,0,255, 0,0,255, 0,0,255, 0,0,255, 255,255,255, 255,255,255, 255,255,255, 255,255,255, 255,0,0, 255,0,0, 255,0,0, 255,0,0,
    };
    auto imgRgb2 = image::create(image::Params(image::Type::RGB,WDims(12,12),rgbbuffer2));

    bool big = false;
	Window window(WindowParams("testex").dims(200,100));
	Window window2(WindowParams("draw").dims(400,400));
	bool closed = false;
	window.onMouseDown([&](MouseEvent e, Point pt) {
		switch(e.button) {
		case MouseButton::left:
			window.setDims(WDims(400,400));
			app.processMessages(); // makes setDims to execute now, before drawing
			window.canvas().drawPoly(points,RGBColor(0,180,0));
			window2.canvas().drawPoly(points,RGBColor(0,0,180));
			imgRgb2->beginDraw();
			imgRgb2->canvas().drawLine(Point(points.size(),0),Point(11,11),RGBColor(255,0,0));
			imgRgb2->endDraw();
			imgRgb2->drawInto(window.canvas(),pt);
			points.clear();
			break;
		case MouseButton::right:
			window.canvas().drawLine(Point(),pt,RGBColor(180,0,0));
			big = ! big;
			if( big ) {
				imgRgb->drawInto(window.canvas(),Rect::square(pt,30));
			} else {
				imgRgb2->drawInto(window.canvas(),pt);
				cout << "dims = " << imgRgb2->dims()
					 << " bpp = " << imgRgb2->bpp() << endl;
			}
			break;
		case MouseButton::middle:
			points.push_back(pt);
		break;
		}
	});
	window.onClose([&closed]() -> bool { closed = true; return true; });
	window.onResize([](WDims d) -> WDims {
		cout << "attempt new dims " << d << endl;
		if( d.width > 600 ) d.width = 600;
		if( d.height > 400 ) d.height = 400;
		return d;
	});
	window.setCursor(Cursor::cross);
	window.setBackground(RGBColor(0,150,0));
	window.onKeyDown([&window](WindowKey key) -> WindowKey {
		if( key == k_RIGHT ) {
			cout << "previous rect " << window.rect() << endl;
			window.setPos(window.pos().addX(5));
		}
		return key;
	});

	TextParams tp;
	tp.color(RGBColor(20,30,140));
	window2.onMouseDown([&window2,&window,&tp,&app](MouseEvent e, Point pt) {
		tp.verticalAlign(cycle(tp.v_align));
		tp.horizontalAlign(cycle(tp.h_align));
		window2.canvas().textRect(Rect::open(pt,WDims(250,20)),"Other TEST!"_dies,tp);
		window2.canvas().fillRect(Rect::square(pt,20),RGBColor(20,30,40));
		window.canvas().drawText(pt,"TEST!"_dies,RGBColor(200,30,40));
		window2.setCursor(Cursor::hand);
		auto screenPos = app.getCursorPos();
		cout << "cursor pos " << screenPos << endl;
		cout << "window pos " << window2.screenToClient(screenPos) << endl;
	});
	window2.onClose([]() -> bool { return false; });

	do {
		app.waitForMessages();
		app.processMessages();
	} while( ! closed );

	cout << "!!!Bye World!!!" << endl;
}
