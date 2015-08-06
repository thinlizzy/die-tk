#include <die-tk.h>

using namespace tk;

int main()
{
	Application app;

	Window window(WindowParams("test clipboard").dims(600,400));
	window.onPaint([](Canvas & canvas, Rect) {
		canvas.drawText(Point(10,10),"click anywhere on the window to display pasted text from clipboard"_ns,RGBColor(0,200,0));
	});
	window.onMouseDown([&](MouseEvent, Point p){
		auto text = app.getClipboardText();
		if( text.empty() ) return;
		// TODO iterate over lines to improve the example. need support to getline or sstream or split iterator to NativeString
		window.canvas().drawText(p,text,RGBColor(200,200,0));
	});
	do {
		app.waitForMessages();
		app.processMessages();
	} while( window.visible() );
}
