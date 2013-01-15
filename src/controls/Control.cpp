#include "Control.h"
#include "../Application.h"

namespace tk {

template<typename CbType>
void callbackImpl(Control & control, void (Application::*appCallback)(std::shared_ptr<Control>,CbType), CbType callback)
{
    Application app;
    (app.*appCallback)(control.shared_from_this(),callback);
}

void Control::onMouse(HandleMouseEvent callback)
{
    callbackImpl(*this,&Application::onMouse,callback);
}

void Control::onKey(ProcessKeyEvent callback)
{
    callbackImpl(*this,&Application::onKey,callback);
}

void Control::onKeypress(ProcessKeypress callback)
{
    callbackImpl(*this,&Application::onKeypress,callback);
}

void Control::onClick(HandleOperation callback)
{
    callbackImpl(*this,&Application::onClick,callback);
}

void Control::onPaint(HandlePaint callback)
{
    callbackImpl(*this,&Application::onPaint,callback);
}

void Control::onChange(HandleOperation callback)
{
    callbackImpl(*this,&Application::onChange,callback);
}
    
}