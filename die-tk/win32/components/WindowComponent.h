#ifndef WINDOW_COMPONENT_H_UH49gfk3n56gh833hh
#define WINDOW_COMPONENT_H_UH49gfk3n56gh833hh

#include <windows.h>
#include <memory>
#include "../../util/optional.h"
#include "../../objects/Dimension.h"

namespace tk {

class WindowImpl;
    
class WindowComponent: public std::enable_shared_from_this<WindowComponent> {
public:
    virtual ~WindowComponent() {}    
    // window calls this for all registered components when it gets a WM_COMMAND message
    // return default optional to continue processing or something else otherwise
    virtual optional<LRESULT> processNotification(UINT message, UINT notification, UINT id, LPARAM lParam) = 0;
    virtual void unregister() = 0;
    virtual WDims payload() = 0;
};

}

#endif
