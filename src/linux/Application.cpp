#include "../Application.h"

#include "ClipboardX11.h"
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
	// TODO implement show console
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


NativeString Application::getClipboardText()
{
	Clipboard clipboard("CLIPBOARD");
	return clipboard.pasteString();
}

}
