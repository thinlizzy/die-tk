#ifndef BALL_H_
#define BALL_H_

#include <die-tk.h>

class Ball {
public:
	enum class Status { noChange, shift, };
	struct Movement {
		Status status;
		tk::Point offset;
	};
private:
	tk::Point coordinates;
	tk::Point movingOffset;
	tk::Rect limits;
	int speed = 2;
	tk::WDims size = tk::WDims::squared(16);
public:
	Ball() = default;
	Movement processKey(tk::WindowKey key);
	Movement processKeyRelease(tk::WindowKey key);
	tk::Rect rect() const;
	void setRight(int x);
	void setBottom(int y);
	void setLimits(tk::WDims dims);
private:
	bool move();
};

#endif /* BALL_H_ */
