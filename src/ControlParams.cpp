#include "ControlParams.h"
#include <ostream>

namespace tk {

std::ostream & operator<<(std::ostream & os, ControlParams const & cp)
{
	os << "start " << cp.start_ << " dims " << cp.dims_;
	if( ! cp.visible_ ) os << " invisible";
	if( cp.backgroundColor_ ) os << " background " << *cp.backgroundColor_;
	if( cp.cursor_ != tk::Cursor::defaultCursor ) os << " cursor " << int(cp.cursor_);
	if( cp.scrollbar_ != tk::Scrollbar::none ) os << " scrollbar " << int(cp.scrollbar_);
	if( cp.autosize_ ) os << " autosize";
	if( ! cp.text_.empty() ) os << " '" << cp.text_ << "'";
	return os;
}

}
