#include "die-tk.h"

using namespace tk;

int main() {
	Application app;

	auto window = Window(WindowParams("ImageCanvas example").start({10,10}).dims({450,300}));

	auto littleBoxCanvas = ImageCanvas::create({20,20});
	littleBoxCanvas->fillRect(Rect::open({},{20,20}),RGBColor(200,0,0));
	littleBoxCanvas->fillRect(Rect::open({5,5},{10,10}),RGBColor(0,200,0));
	auto littleBox = littleBoxCanvas->finishAndCreateImage();

	auto littleBoxCanvasT = ImageCanvas::create({20,20},true);
	littleBoxCanvasT->fillRect(Rect::open({},{20,20}),RGBColor(200,0,0));
	littleBoxCanvasT->fillRect(Rect::open({5,5},{10,10}),RGBColor(0,200,0));
	auto littleBoxT = littleBoxCanvasT->finishAndCreateImage();

	auto imageCanvasT = ImageCanvas::create({90,100},true);
	imageCanvasT->roundRect(Rect::open({0,0},{90,100}),{20,30},RGBColor(200,0,0),RGBColor(200,0,0));
	imageCanvasT->fillRect(Rect::open({25,35},{40,30}),RGBColor(0,200,0));
	imageCanvasT->drawImage(littleBox,{35,40});
	auto imageT = imageCanvasT->finishAndCreateImage();

	// TODO colors changed when drawing transparent images on transparent images
	auto imageCanvasTonT = ImageCanvas::create({90,100},true);
	imageCanvasTonT->drawImage(imageT,{0,0});
	imageCanvasTonT->drawLine({10,10},{80,90},RGBColor(0,0,200));
	imageCanvasTonT->drawLine({80,10},{10,90},RGBColor(0,0,200));
	auto imageTonT = imageCanvasTonT->finishAndCreateImage();

	auto imageCanvasTonTopaque = ImageCanvas::create({90,100});
	imageCanvasTonTopaque->drawImage(imageT,{0,0});
	imageCanvasTonTopaque->drawImage(littleBoxT,{15,40});
	imageCanvasTonTopaque->drawImage(littleBox,{55,40});
	auto imageTonTopaque = imageCanvasTonTopaque->finishAndCreateImage();

	// TODO colors changed when drawing transparent images on transparent images
	auto imageCanvasTonTopaqueW = ImageCanvas::create({100,100},true);
	imageCanvasTonTopaqueW->fillRect(Rect::open({0,0},{100,100}),RGBColor(255,255,255));
	imageCanvasTonTopaqueW->drawImage(imageT,{5,0});
	imageCanvasTonTopaqueW->drawImage(littleBoxT,{20,40});
	imageCanvasTonTopaqueW->drawImage(littleBox,{60,40});
	auto imageTonTopaqueW = imageCanvasTonTopaqueW->finishAndCreateImage();

	window.onPaint([&](Canvas & canvas, Rect rect) {
		canvas.fillRect(rect,RGBColor(0,0,100));
		canvas.drawImage(littleBox,Point(30,30));
		canvas.drawImage(littleBox,Point(30,60));
		canvas.drawImage(littleBox,Point(60,30));
		canvas.drawImage(littleBox,Point(60,60));
		canvas.drawImage(littleBoxT,Point(30,90));
		canvas.drawImage(littleBoxT,Point(30,120));
		canvas.drawImage(littleBoxT,Point(60,90));
		canvas.drawImage(littleBoxT,Point(60,120));

		canvas.drawImage(imageT,Point(100,30));
		canvas.drawImage(imageTonT,Point(210,30));
		canvas.drawImage(imageTonTopaque,Point(210,150));
		canvas.drawImage(imageTonTopaqueW,Point(100,150));
		// manually reproduce imageTonT
		canvas.roundRect(Rect::open({300,30},{90,100}),{20,30},RGBColor(200,0,0),RGBColor(200,0,0));
		canvas.fillRect(Rect::open({325,65},{40,30}),RGBColor(0,200,0));
		canvas.drawImage(littleBox,{335,70});
		canvas.drawLine({310,40},{380,120},RGBColor(0,0,200));
		canvas.drawLine({380,40},{310,120},RGBColor(0,0,200));
	});

	do {
		app.waitForMessages();
		app.processMessages();
	} while( window.visible() );
}
