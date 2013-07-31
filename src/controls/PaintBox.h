#ifndef PAINTBOX_H_fkkkkkkkk453432984328fh
#define PAINTBOX_H_fkkkkkkkk453432984328fh

#include "../Control.h"

namespace tk {

class PaintBox: public Control {
public:
    PaintBox() = default;
    PaintBox(Window & parent, ControlParams const & params = ControlParams());
    
    using Control::onPaint;
};

}

#endif
