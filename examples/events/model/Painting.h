#ifndef PAINTING_H_
#define PAINTING_H_

#include <die-tk.h>

class Painting {
	tk::Point coordinates;
	bool drawing;
	tk::image::Ptr imgCanvas;
	tk::Pen pen;
public:
	Painting(tk::WDims dims);
	Painting(Painting &&) = default;
	void setCrosshair(tk::Point point);
	void penDown();
	void penUp();
	void removeCrosshair();
	void resize(tk::WDims dims);

	tk::Point posCrosshair() const { return coordinates; }
	bool isDrawing() const { return drawing; }
	tk::image::Ptr const & imageCanvas() const { return imgCanvas; }
	tk::Pen drawingPen() const { return pen; }
};

#endif
