#ifndef PAINTBOX_H_DIE_TK_2016_08
#define PAINTBOX_H_DIE_TK_2016_08

#include "../Control.h"

namespace tk {

class PaintBox: public Control {
public:
    PaintBox() = default;
    PaintBox(Window & parent, ControlParams const & params = ControlParams());
    PaintBox clone() const;
    
    using Control::onPaint;

    bool transparent() const;

    void setTransparentBackground();
};

}

#endif
