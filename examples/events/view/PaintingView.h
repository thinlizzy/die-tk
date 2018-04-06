#ifndef PAINTINGVIEW_H_
#define PAINTINGVIEW_H_

#include "die-tk.h"
#include "../model/Painting.h"

class PaintingView {
	Painting & painting;
	tk::Window & window;
	tk::Pen crossHairPen = tk::RGBColor(30,200,30);
	int crossHairRadius = 10;
public:
	PaintingView(Painting & painting, tk::Window & window);
	void drawCrosshair(tk::Point newPoint);
	void move(tk::Point oldPoint, tk::Point newPoint);
	void updatePen();
	void eraseCrosshair(tk::Point oldPoint);
	void redraw(tk::Canvas & canvas, tk::Rect rect);
private:
	void drawX(tk::Point newPoint);
};

#endif
