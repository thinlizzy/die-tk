#include "CustomControlImpl.h"

namespace tk {

CustomControlImpl::CustomControlImpl(ControlParams const & controlParams):
	rect(Rect::closed(controlParams.start_, controlParams.dims_)),
	visible(controlParams.visible_) {
}

}