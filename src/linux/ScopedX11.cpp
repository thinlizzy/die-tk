#include "ScopedX11.h"
#include "ResourceManager.h"

namespace tk {

namespace scoped {

void DestroyPixmap::operator()(::Pixmap pixmap) const
{
	if (pixmap) XFreePixmap(resourceManager.dpy, pixmap);
}

void DestroyCursor::operator()(::Cursor cursor) const
{
	if (cursor) XFreeCursor(resourceManager.dpy, cursor);
}

}

}
