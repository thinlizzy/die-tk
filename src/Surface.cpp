#include "Surface.h"

namespace tk {

Surface::~Surface()
{}

int Surface::x() const
{
	return pos().x;
}

int Surface::y() const
{
	return pos().y;
}

Point Surface::pos() const
{
	return rect().pos();
}

int Surface::width() const
{
	return dims().width;
}

int Surface::height() const
{
	return dims().height;
}

WDims Surface::dims() const
{
	return rect().dims();
}

void Surface::setEnabled(bool enabled)
{
    if( enabled ) enable(); else disable();
}

void Surface::setVisible(bool visible)
{
    if( visible ) show(); else hide();
}

void Surface::clear(RGBColor const & color)
{
    auto & canvas1 = canvas();
	canvas1.fillRect(Rect::closed(Point(0,0),dims()),color);
}


}