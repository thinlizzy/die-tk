#ifndef MEMO_H_kjfdsfk34
#define MEMO_H_kjfdsfk34

#include "../Control.h"

namespace tk {

class Memo: public Control {
public:
    Memo() = default;
    Memo(Window & parent, ControlParams const & params = ControlParams());
    
	void setReadOnly(bool readOnly);
    
	using Control::getText;
	using Control::setText;

    using Control::onKey;
    using Control::onKeypress;
};

}

#endif
