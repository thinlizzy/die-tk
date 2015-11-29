#include "../Control.h"
#include "../Window.h"

namespace tk {
    
Window Control::getParent() const
{
    ResourceManagerSingleton resourceManager;
    return Window(resourceManager->findWindow(impl->getParentHandle()));
}

}