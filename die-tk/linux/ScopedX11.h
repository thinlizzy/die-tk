#ifndef LINUX_SCOPEDX11_H_DIETK_2015_06_21
#define LINUX_SCOPEDX11_H_DIETK_2015_06_21

#include <X11/Xlib.h>
#include "../util/ScopedHandle.h"

namespace tk {

namespace scoped {

struct DestroyPixmap {
	void operator()(::Pixmap pixmap) const;
};
using Pixmap = Handle<::Pixmap,DestroyPixmap>;

struct DestroyCursor {
	void operator()(::Cursor cursor) const;
};
using Cursor = Handle<::Cursor,DestroyCursor>;

struct DestroyWindow {
	void operator()(::Window windowId) const;
};
using Window = Handle<::Window,DestroyWindow>;

}

}

#endif /* LINUX_SCOPEDX11_H_ */
