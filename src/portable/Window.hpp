#include "../Window.h"
#include "../Control.h"

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

die::NativeString Window::selectFile(SelectFileParams const & params)
{
    return getImpl().selectFile(params);
}

std::vector<die::NativeString> Window::selectFiles(SelectFileParams const & params)
{
    return getImpl().selectFiles(params);    
}

die::NativeString Window::selectFileForSave(SelectFileParams const & params)
{
    return getImpl().selectFileForSave(params);    
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
