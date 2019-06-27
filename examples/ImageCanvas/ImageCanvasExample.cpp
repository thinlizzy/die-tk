#include "die-tk.h"

using namespace tk;

int main() {
	Application app;

	auto window = Window(WindowParams("ImageCanvas example").start({10,10}).dims({320,400}));

	auto imageCanvas = ImageCanvas::create({20,20});
	imageCanvas->fillRect(Rect::open({},{20,20}),RGBColor(200,0,0));
	imageCanvas->fillRect(Rect::open({5,5},{10,10}),RGBColor(0,200,0));
	auto image = imageCanvas->finishAndCreateImage();

	auto imageCanvasT = ImageCanvas::create({90,100},true);
	imageCanvasT->roundRect(Rect::open({0,0},{90,100}),{20,30},RGBColor(200,0,0),RGBColor(200,0,0));
	imageCanvasT->fillRect(Rect::open({25,35},{40,30}),RGBColor(0,200,0));
	auto imageT = imageCanvasT->finishAndCreateImage();

	auto imageCanvasTonT = ImageCanvas::create({90,100},true);
	imageCanvasTonT->drawImage(imageT,{0,0});
	imageCanvasTonT->drawLine({10,10},{80,90},RGBColor(0,0,200));
	imageCanvasTonT->drawLine({80,10},{10,90},RGBColor(0,0,200));
	auto imageTonT = imageCanvasTonT->finishAndCreateImage();

	auto imageCanvasTonTopaque = ImageCanvas::create({90,100});
	imageCanvasTonTopaque->drawImage(imageT,{0,0});
	auto imageTonTopaque = imageCanvasTonTopaque->finishAndCreateImage();

	window.onPaint([&](Canvas & canvas, Rect rect) {
		canvas.fillRect(rect,RGBColor(0,0,100));
		canvas.drawImage(image,Point(30,30));
		canvas.drawImage(image,Point(30,60));
		canvas.drawImage(image,Point(60,30));
		canvas.drawImage(image,Point(60,60));
		canvas.drawImage(imageT,Point(100,30));
		canvas.drawImage(imageTonT,Point(200,30));
		canvas.drawImage(imageTonTopaque,Point(200,150));
	});

	do {
		app.waitForMessages();
		app.processMessages();
	} while( window.visible() );
}
