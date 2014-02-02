#include "Control.h"

namespace tk {

Control::~Control()
{}

int Control::x() const
{
	return pos().x;
}

int Control::y() const
{
	return pos().y;
}

Point Control::pos() const
{
	return rect().pos();
}

int Control::width() const
{
	return dims().width;
}

int Control::height() const
{
	return dims().height;
}

WDims Control::dims() const
{
	return rect().dims();
}

void Control::setEnabled(bool enabled)
{
    if( enabled ) enable(); else disable();
}

void Control::setVisible(bool visible)
{
    if( visible ) show(); else hide();
}

void Control::clear(RGBColor const & color)
{
    auto & canvas1 = canvas();
	canvas1.fillRect(Rect::closed(Point(0,0),dims()),color);
}


}