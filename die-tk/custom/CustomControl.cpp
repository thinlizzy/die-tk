#include "CustomControl.h"

namespace tk {

CustomControl::CustomControl(Surface & parent):
	parent(parent) {
}

CustomControl::~CustomControl() {
}

void CustomControl::addToParent(std::shared_ptr<CustomControlImpl> const & controlImpl) {
	parent.addCustomControlImpl(controlImpl);
	invalidateIfVisible();
}

void CustomControl::invalidate() {
	parent.invalidate(rect());
}

void CustomControl::invalidateIfVisible() {
	if( visible() ) {
		invalidate();
	}
}

int CustomControl::x() const {
	return rect().left;
}

int CustomControl::y() const {
	return rect().top;
}

Point CustomControl::pos() const {
	return rect().topLeft();
}

int CustomControl::width() const {
	return rect().width();
}

int CustomControl::height() const {
	return rect().height();
}

WDims CustomControl::dims() const {
	return rect().dims();
}

Rect CustomControl::rect() const {
	return controlImpl().rect;
}

void CustomControl::setPos(Point pos) {
	setRect(rect().move(pos));
}

void CustomControl::setDims(WDims dims) {
	setRect(rect().resize(dims));
}

void CustomControl::setRect(Rect rect) {
	if( visible() ) {
		invalidate();
		controlImpl().rect = rect;
		invalidate();
	} else {
		controlImpl().rect = rect;
	}
}

void CustomControl::show() {
	if( ! visible() ) {
		controlImpl().visible = true;
		invalidate();
	}
}

void CustomControl::hide() {
	if( visible() ) {
		controlImpl().visible = false;
		invalidate();
	}
}

bool CustomControl::visible() const {
	return controlImpl().visible;
}

}