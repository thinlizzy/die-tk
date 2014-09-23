#include <die-tk.h>
#include <iostream>
using namespace std;
using namespace tk;

int main()
{
	cout << "!!!Hello World!!!" << endl;

	Window window(WindowParams("testex").dims(200,100));
	bool closed = false;
	window.onMouseDown([&window](MouseEvent e,Point){
		if(e.button == mb_left)
			window.setDims(WDims(400,400));
	});
	window.onClose([&closed]() -> bool { closed = true; return true; });
	window.onResize([](WDims d) -> WDims {
		cout << "attempt new dims " << d << endl;
		if( d.width > 600 ) d.width = 600;
		if( d.height > 400 ) d.height = 400;
		return d;
	});
	Application app;
	do {
		app.waitForMessages();
		app.processMessages();
	} while( ! closed );

	cout << "!!!Bye World!!!" << endl;
}
