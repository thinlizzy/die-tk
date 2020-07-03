#include "die-tk.h"

using namespace tk;

int main() {
	auto window = Window(WindowParams("custom controls").dims({300,200}));

	auto label = Label(window, ControlParams()
		.text("first label")
		.color({200,0,0})
		.backgroundColor({255,255,255})
		.start({10,10})
		.dims({20,30}));

	// unnamed autosized labels
	Label(window, ControlParams()
		.text("second label")
		.color({0,0,200})
		.start({10,50})
		.autosize());
	Label(window, ControlParams()
		.text("01234567890!/|abcdefghijklmnopqrstuvxyz")
		.color({0,200,0})
		.backgroundColor({200,0,0})
		.start({10,70})
		.autosize());
	Label(window, ControlParams()
		.text("01234567890!/|abcdefghijklmnopqrstuvxyz")
		.color({0,200,0})
		.start({100,50})
		.autosize());

	window.onKeypress([&label](char key) {
		switch(key) {
			case 'c':
			case 'C':
				label.setColor({0,255,0});
				break;
			case 'd':
			case 'D':
				label.setDims(label.dims().addWidth(8));
				break;
		}
		return key;
	});

	Application app;
	do {
		app.waitForMessages();
		app.processMessages();
	} while( window.visible() );
}
