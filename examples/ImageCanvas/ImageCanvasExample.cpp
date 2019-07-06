#include <ostream>
#include "die-tk.h"

using namespace tk;

int main() {
	Application app;

	auto window = Window(WindowParams("ImageCanvas example").start({10,10}).dims({450,300}));

	std::cerr << "creating littleBox image" << std::endl;
	auto littleBoxCanvas = ImageCanvas::create({20,20});
	littleBoxCanvas->fillRect(Rect::open({},{20,20}),RGBColor(200,0,0));
	littleBoxCanvas->fillRect(Rect::open({5,5},{10,10}),RGBColor(0,200,0));
	auto littleBox = littleBoxCanvas->finishAndCreateImage();

	std::cerr << "creating littleBoxT image" << std::endl;
	auto littleBoxCanvasT = ImageCanvas::create({20,20},true);
	littleBoxCanvasT->fillRect(Rect::open({},{20,20}),RGBColor(200,0,0));
	littleBoxCanvasT->fillRect(Rect::open({5,5},{10,10}),RGBColor(0,200,0));
	auto littleBoxT = littleBoxCanvasT->finishAndCreateImage();

	std::cerr << "creating imageT" << std::endl;
	auto imageCanvasT = ImageCanvas::create({90,100},true);
	imageCanvasT->roundRect(Rect::open({0,0},{90,100}),{20,30},RGBColor(200,0,0),RGBColor(200,0,0));
	imageCanvasT->fillRect(Rect::open({25,35},{40,30}),RGBColor(0,200,0));
	imageCanvasT->drawImage(littleBox,{35,40});
	auto imageT = imageCanvasT->finishAndCreateImage();

	std::cerr << "creating imageTonT" << std::endl;
	auto imageCanvasTonT = ImageCanvas::create({90,100},true);
	imageCanvasTonT->drawImage(imageT,{0,0});
	imageCanvasTonT->drawLine({10,10},{80,90},RGBColor(0,0,200));
	imageCanvasTonT->drawLine({80,10},{10,90},RGBColor(0,0,200));
	auto imageTonT = imageCanvasTonT->finishAndCreateImage();

	std::cerr << "creating imageTonTopaque" << std::endl;
	auto imageCanvasTonTopaque = ImageCanvas::create({90,100});
	imageCanvasTonTopaque->drawImage(imageT,{0,0});
	imageCanvasTonTopaque->drawImage(littleBoxT,{15,40});
	imageCanvasTonTopaque->drawImage(littleBox,{55,40});
	auto imageTonTopaque = imageCanvasTonTopaque->finishAndCreateImage();

	std::cerr << "creating imageTonTfillW" << std::endl;
	auto imageCanvasTonTfillW = ImageCanvas::create({100,100},true);
	imageCanvasTonTfillW->fillRect(Rect::open({0,0},{100,100}),RGBColor(255,255,255));
	imageCanvasTonTfillW->drawImage(imageT,{5,0});
	imageCanvasTonTfillW->drawImage(littleBoxT,{20,40});
	imageCanvasTonTfillW->drawImage(littleBox,{60,40});
	auto imageTonTfillW = imageCanvasTonTfillW->finishAndCreateImage();

	std::cerr << "creating almost empty transparent image" << std::endl;
	auto imageCanvasEmptyT = ImageCanvas::create({10,10},true);
	imageCanvasEmptyT->drawLine({0,0},{9,9},RGBColor(0,200,200));
	auto imageEmptyT = imageCanvasEmptyT->finishAndCreateImage();

	std::cerr << "done creating images" << std::endl;

	window.onPaint([&](Canvas & canvas, Rect rect) {
		canvas.fillRect(Rect::open({0,0},window.dims()),RGBColor(0,0,100));
		canvas.fillRect(Rect::open({0,100},window.dims().setHeight(100)),RGBColor(0,0,0));

		canvas.drawText({20,15},"little boxes",RGBColor(255,255,255));
		canvas.drawImage(littleBox,Point(30,30));
		canvas.drawImage(littleBox,Point(30,60));
		canvas.drawImage(littleBox,Point(60,30));
		canvas.drawImage(littleBox,Point(60,60));
		canvas.drawText({20,85},"little boxes T",RGBColor(255,255,255));
		canvas.drawImage(littleBoxT,Point(30,100));
		canvas.drawImage(littleBoxT,Point(30,130));
		canvas.drawImage(littleBoxT,Point(60,100));
		canvas.drawImage(littleBoxT,Point(60,130));

		canvas.drawText({100,15},"imageT",RGBColor(255,255,255));
		canvas.drawImage(imageT,Point(100,30));
		canvas.drawText({210,15},"imageTonT",RGBColor(255,255,255));
		canvas.drawImage(imageTonT,Point(210,30));
		canvas.drawText({210,135},"imageTonTopaque",RGBColor(255,255,255));
		canvas.drawImage(imageTonTopaque,Point(210,150));
		canvas.drawText({100,135},"imageTonTfillW",RGBColor(255,255,255));
		canvas.drawImage(imageTonTfillW,Point(100,150));

		canvas.drawImage(imageEmptyT,Point(10,200));

		// manually reproduce imageTonT
		canvas.drawText({300,15},"manual imageTonT",RGBColor(255,255,255));
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
