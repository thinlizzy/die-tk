#include "Label.h"
#include <memory>

namespace tk {

Label::Label(Surface & parent, ControlParams const & controlParams):
	CustomControl(parent),
	labelImpl(std::make_shared<LabelImpl>(controlParams)) {
	redimAutosize();
	addToParent(labelImpl);
}

LabelImpl & Label::controlImpl() {
	return *labelImpl;
}

LabelImpl const & Label::controlImpl() const {
	return *labelImpl;
}

void Label::setText(NativeString const & text) {
	labelImpl->text = text;
	invalidateIfVisible();
	if( redimAutosize() ) {
		invalidateIfVisible();
	}
}

NativeString Label::text() const {
	return labelImpl->text;
}

void Label::setColor(RGBColor color) {
	labelImpl->color = color;
	invalidateIfVisible();
}

RGBColor Label::color() const {
	return labelImpl->color;
}

void Label::setBackgroundColor(optional<RGBColor> color) {
	labelImpl->backgroundColor = color;
	invalidateIfVisible();
}

optional<RGBColor> Label::backGroundColor() const {
	return labelImpl->backgroundColor;
}

void Label::setAutosize(bool enable) {
	if( enable == autosize() ) return;
	labelImpl->autosize = enable;
	if( redimAutosize() ) {
		invalidateIfVisible();
	}
}

bool Label::autosize() const {
	return labelImpl->autosize;
}

bool Label::redimAutosize() {
	if( ! autosize() ) return false;
	labelImpl->rect = labelImpl->rect.resize(parent.canvas().measureText(labelImpl->text));
	return true;
}

}