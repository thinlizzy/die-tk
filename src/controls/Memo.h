#ifndef MEMO_H_kjfdsfk34
#define MEMO_H_kjfdsfk34

#include "../Control.h"
#include "../StringList.h"

namespace tk {

class Memo: public Control {
public:
    Memo() = default;
    Memo(Window & parent, ControlParams const & params = ControlParams());
    Memo clone() const;
    
	void setReadOnly(bool readOnly);
    void setTextAlign(HTextAlign alignment);
    
    StringList & lines();
    
    void scrollTo(unsigned lines);
    
    using Control::onKey;
    using Control::onKeypress;
};

}

#endif
