#ifndef CONTROL_REFERENCE_H_kf34252943mdvj48dhdrgg3213213
#define CONTROL_REFERENCE_H_kf34252943mdvj48dhdrgg3213213

#include "Control.h"

namespace tk {

class ControlRef: public Control {
public:
    explicit ControlRef(Control const & control);
    ControlRef(ControlRef const & cr);
    ControlRef & operator=(ControlRef const & cr);
};

}

#endif