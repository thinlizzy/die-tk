#ifndef CONTROL_H_gfh430hm4bg73bg93gllhr52bzq
#define CONTROL_H_gfh430hm4bg73bg93gllhr52bzq

#include "Surface.h"
#include "ControlParams.h"

namespace tk {

class Window;

/* base class for all widgets, including windows.
 * 
 * Control objects acts as references. they are not scoped. 
 * once a control is created, its ownership is transferred to its parent window.
 * only the owner window can remove a control from it by explictly calling remove()
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

