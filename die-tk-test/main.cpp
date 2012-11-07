#include "../src/Application.h"
#include "../src/Callbacks.h"
#include "../src/Window.h"
#include "../src/Canvas.h"
#include "../../libimage/src/image.h"

#include <iostream>

int main()
{
	{
		using namespace tk;
		Application app;

		app.showConsole();

		auto window1 = app.createWindow();
		app.onClose(window1,[]() -> bool {
			std::cout << "window was closed!" << std::endl;
			return true;
		});
		window1->canvas().setBrush(tk::RGBColor(0,100,0));
		window1->canvas().setPen(tk::RGBColor(0,100,0));
		app.onPaint(window1,[&](Canvas & canvas, Rect rect) {
			canvas.fillRect(rect);
		});
		auto button1 = app.createButton(window1,ControlParams().text("click me").start(20,20));
		app.onClick(button1,[]() {
			std::cout << "clicked me" << std::endl;
		});

		app.onMouse(button1,[](MouseEvent evt, Point pt) {
			std::cout << "clicked me with " << evt.type << " at " << pt << std::endl;
		});

		img::Image image("DIEGO1.jpg");
		auto imagepb = app.createPaintBox(window1,ControlParams().start(400,100).dims(100,100));
		app.onPaint(imagepb,[&](Canvas & canvas, Rect rect) {
			auto imageHeader = image.getWindowSystemHeader();
			auto imageBits = image.rawBits();
			canvas.drawImage(rect,tk::Point(0,0),imageBits,imageHeader);
		});


		auto window2 = app.createWindow(WindowParams().text("second window"));
		window2->clear(RGBColor(100,0,0));
		app.onClose(window2,[]() -> bool {
			static bool x = false;
			if( x ) {
				std::cout << "second window was closed!" << std::endl;
				return true;
			}
			x = true;
			std::cout << "try again!" << std::endl;
			return false;
		});

		auto paintboxG = app.createPaintBox(window2,ControlParams().start(window2->width()/2,window2->height()/2).dims(100,100));
		paintboxG->setBackground(tk::RGBColor(200,100,0));

		auto imageCtl = app.createImage(window2,ControlParams().start(300,100));
		imageCtl->setImage(image.rawBits(),image.getWindowSystemHeader());

		auto paintboxB = app.createPaintBox(window2,ControlParams().start(280,80).dims(100,100));
		paintboxB->setBackground(tk::RGBColor(0,100,200));

		auto buttonLeft = app.createButton(window2,ControlParams().text("left!").start(260,130));
		auto buttonRight = app.createButton(window2,ControlParams().text("right!").start(900,130));

        paintboxG->bringToFront();      // that one is hidden by the image control
        paintboxB->bringToFront();      // that is partially hidden

        buttonRight->bringToFront();

        auto treeView = app.createTreeView(window1,ControlParams().start(10,100).dims(100,300));
        auto itemTree = treeView->items();
        auto it1 = itemTree.add(TreeView::Item().setText("test"));
        itemTree.add(TreeView::Item().setText("second item"));
        auto it2 = itemTree.add(TreeView::Item().setText("third item"));
        it1.children().add(TreeView::Item().setText("child"));
        it1.children().add(TreeView::Item().setText("child2"));

		while( window1->state() == ws_visible || window2->state() == ws_visible ) {
			app.waitForMessages();
			app.processMessages();
		}
	}

	std::cout << "type something and press enter to quit ";
	int a;
	std::cin >> a;
}
