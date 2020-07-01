#include "die-tk.h"

using namespace tk;

int main() {
	auto window = Window(WindowParams("custom controls"));

	auto label = Label(window, ControlParams()
		.text("first label")
		.color({200,0,0})
		.backgroundColor({255,255,255})
		.start({10,10})
		.dims({20,20}));

	// unnamed autosized label
	Label(window, ControlParams()
		.text("second label")
		.color({200,0,0})
		.start({10,30})
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