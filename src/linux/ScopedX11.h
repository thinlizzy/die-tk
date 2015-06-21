#ifndef LINUX_SCOPEDX11_H_DIETK_2015_06_21
#define LINUX_SCOPEDX11_H_DIETK_2015_06_21

#include "../util/ScopedHandle.h"
#include <X11/Xlib.h>

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

}

}

#endif /* LINUX_SCOPEDX11_H_ */
