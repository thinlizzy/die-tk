#ifndef BALLVIEW_H_
#define BALLVIEW_H_

#include <die-tk.h>
#include "../model/Ball.h"

class BallView {
	Ball & ball;
	tk::Window & window;
	tk::RGBColor ballColor1,ballColor2,backgroundColor;
	int drawOff = 4;
public:
	BallView(Ball & ball, tk::Window & window);
	void moveBall(tk::Point offset);
	void drawIfInRect(tk::Rect rect);
private:
	void draw();
};

#endif /* BALLVIEW_H_ */
