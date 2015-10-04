#include "Ball.h"

auto Ball::processKey(tk::WindowKey key) -> Movement
{
	bool moving = true;
	auto last = coordinates;
	switch(key) {
	case tk::k_LEFT:
		movingOffset.x = -speed;
		break;
	case tk::k_RIGHT:
		movingOffset.x = +speed;
		break;
	case tk::k_UP:
		movingOffset.y = -speed;
		break;
	case tk::k_DOWN:
		movingOffset.y = +speed;
		break;
	default:
		moving = false;
	}
	if( moving && move() ) return { Status::shift, coordinates-last };
	return {};
}

bool Ball::move()
{
	auto last = coordinates;
	coordinates += movingOffset;
	coordinates = limits.fitPoint(coordinates);
	return coordinates != last;
}

auto Ball::processKeyRelease(tk::WindowKey key) -> Movement
{
	// could be smoother in many ways, but I am trying to make this a simple example
	switch(key) {
	case tk::k_LEFT:
	case tk::k_RIGHT:
		movingOffset.x = 0;
		break;
	case tk::k_UP:
	case tk::k_DOWN:
		movingOffset.y = 0;
		break;
	}
	if( movingOffset == tk::Point(0,0) ) return {};

	return { Status::shift, movingOffset };
}

tk::Rect Ball::rect() const
{
	return tk::Rect::closed(coordinates,size);
}

void Ball::setRight(int x)
{
	coordinates.x = x - size.width;
}

void Ball::setBottom(int y)
{
	coordinates.y = y - size.height;
}

void Ball::setLimits(tk::WDims dims)
{
	limits = tk::Rect::closed(tk::Point(0,0),dims - size);
}
