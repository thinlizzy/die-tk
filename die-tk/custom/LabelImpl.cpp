#include "LabelImpl.h"

namespace tk {

LabelImpl::LabelImpl(ControlParams const & controlParams):
	CustomControlImpl(controlParams),
	text(controlParams.text_),
	color(controlParams.color_),
	backgroundColor(controlParams.backgroundColor_),
	autosize(controlParams.autosize_) {
}

void LabelImpl::draw(Canvas & canvas, Rect rect) const {

	// TODO clamp text in (local) rect
	canvas.textRect(this->rect,text,TextParams().color(color).background(backgroundColor));
}

}
