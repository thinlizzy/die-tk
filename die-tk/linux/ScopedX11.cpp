#include "ScopedX11.h"
#include "ResourceManager.h"

namespace tk {

namespace scoped {

void DestroyPixmap::operator()(::Pixmap pixmap) const
{
	ResourceManagerSingleton resourceManager;
	if( pixmap ) XFreePixmap(resourceManager->dpy, pixmap);
}

void DestroyCursor::operator()(::Cursor cursor) const
{
	ResourceManagerSingleton resourceManager;
	if( cursor ) XFreeCursor(resourceManager->dpy, cursor);
}

void DestroyWindow::operator()(::Window windowId) const
{
	ResourceManagerSingleton resourceManager;
	if( windowId ) XDestroyWindow(resourceManager->dpy, windowId);
}

}

}
