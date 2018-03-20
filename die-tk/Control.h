#ifndef CONTROL_H_gfh430hm4bg73bg93gllhr52bzq
#define CONTROL_H_gfh430hm4bg73bg93gllhr52bzq

#include "Surface.h"
#include "ControlParams.h"

namespace tk {

class Window;

/*
 * Control objects are not scoped, but they act as references.
 * Once a control is created, its ownership is transferred to its parent window.
 * Only the parent window can remove a control from it by explicitly calling remove()
 */
class Control: public Surface {
    friend class Window;
protected:
    Control() = default;
public:
    Control(Control const &) = default;
    Control & operator=(Control const &) = default;
    Control(Control &&) = default;
    Control & operator=(Control &&) = default;

    Window getParent() const;
};

}

#endif

