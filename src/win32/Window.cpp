#include "../Window.h"
#include "ResourceManager.h"
#include "controls/NativeControlWin32.h"
#include "WindowImplWin32.h"

namespace tk {
    
Window::Window(WindowParams const & params)
{
    auto windowImpl = std::make_shared<WindowImpl>(params);
    impl = windowImpl;
    resourceManager.registerWindow(windowImpl);
}

Window::Window(std::shared_ptr<WindowImpl> impl)
{
    this->impl = impl;
}

Window::~Window()
{   
}

void Window::remove(Control & control)
{
    getImpl().unregisterControl(control.impl);
    control.impl.reset();
}

WindowImpl & Window::getImpl()
{
    return static_cast<tk::WindowImpl &>(*impl);
}

int Window::state()
{
    return getImpl().state();
}

AllowOperation Window::onClose(AllowOperation callback)
{
    return getImpl().onClose(callback);
}

ProcessResize Window::onResize(ProcessResize callback)
{
    return getImpl().onResize(callback);
}

HandleEvent Window::onUserEvent(HandleEvent callback)
{
    return getImpl().onUserEvent(callback);
}

}
