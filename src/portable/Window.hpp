#include "../Window.h"
#include "../Control.h"

namespace tk {
    
Window::Window(WindowParams const & params)
{
    auto windowImpl = std::make_shared<WindowImpl>(params);
    impl = windowImpl;
    ResourceManagerSingleton resourceManager;
    resourceManager->registerWindow(windowImpl);
}

Window::Window(std::shared_ptr<WindowImpl> impl)
{
    this->impl = impl;
}

Window::~Window()
{   
	if( impl && impl.unique() ) {
        ResourceManagerSingleton resourceManager;
		resourceManager->unregisterWindow(getImplPtr());
	}
}

void Window::remove(Control & control)
{
    getImpl().unregisterControl(control.impl);
}

std::shared_ptr<WindowImpl> Window::getImplPtr()
{
    return std::static_pointer_cast<tk::WindowImpl>(impl);
}

WindowImpl & Window::getImpl()
{
    return static_cast<tk::WindowImpl &>(*impl);
}

WindowImpl const & Window::getImpl() const
{
    return static_cast<tk::WindowImpl &>(*impl);    
}

int Window::state() const
{
    return getImpl().state();
}

void Window::setBorders(bool value)
{
    return getImpl().setBorders(value);
}

AllowOperation Window::onClose(AllowOperation callback)
{
    return getImpl().onClose(callback);
}

ProcessResize Window::onResize(ProcessResize callback)
{
    return getImpl().onResize(callback);
}

HandleResize Window::afterResize(HandleResize callback)
{
    return getImpl().afterResize(callback);
}

HandleEvent Window::onUserEvent(HandleEvent callback)
{
    return getImpl().onUserEvent(callback);
}

}
