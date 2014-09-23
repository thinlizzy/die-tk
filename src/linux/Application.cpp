#include "../Application.h"
#include "ResourceManager.h"

namespace tk {

void Application::processMessages()
{
	resourceManager.processMessages();
}


void Application::waitForMessages()
{
    XEvent e;
    XPeekEvent(resourceManager.dpy, &e);
}


void Application::showConsole()
{
	throw "Application::showConsole() not implemented";
}


void Application::showMessage(die::NativeString const & message)
{
	throw "Application::showMessage() not implemented";
}


Point Application::getCursorPos() const
{
	throw "Application::getCursorPos() not implemented";
}


die::NativeString Application::getClipboardText()
{
	throw "Application::getClipboardText() not implemented";
}

}
