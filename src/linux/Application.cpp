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
	Window root_return, child_return;
	int root_x_return, root_y_return;
	unsigned int mask_return;
	Point result;
	XQueryPointer(resourceManager.dpy, resourceManager.root(),
		&root_return, &child_return, &root_x_return, &root_y_return,
		&result.x, &result.y,
		&mask_return);
	return result;
}


die::NativeString Application::getClipboardText()
{
	throw "Application::getClipboardText() not implemented";
}

}
