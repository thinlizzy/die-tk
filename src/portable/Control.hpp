#include "../Control.h"
#include "../Window.h"

namespace tk {
    
Window Control::getParent() const
{
    return Window(resourceManager.findWindow(impl->getParentHandle()));
}

}