#include "Window.h"
#include "Application.h"

namespace tk {

template<typename CbType>
void callbackImpl(Window & window, void (Application::*appCallback)(WindowPtr,CbType), CbType callback)
{
    Application app;
    (app.*appCallback)(std::dynamic_pointer_cast<Window>(window.shared_from_this()),callback);
}

void Window::onClose(AllowOperation callback)
{
    callbackImpl(*this,&Application::onClose,callback);
}

void Window::onResize(ProcessResize callback)
{
    callbackImpl(*this,&Application::onResize,callback);
}

void Window::onUserEvent(HandleEvent callback)
{
    callbackImpl(*this,&Application::onUserEvent,callback);
}

}