#ifndef DIE_TK_CUSTOMCONTROL_2020_28_06_H
#define DIE_TK_CUSTOMCONTROL_2020_28_06_H

#include <memory>
#include "CustomControlImpl.h"
#include "../Canvas.h"
#include "../NativeString.h"
#include "../Surface.h"
#include "../objects/Dimension.h"
#include "../objects/Point.h"

namespace tk {

class CustomControl {
protected:
	Surface & parent;

	CustomControl(Surface & parent);

	void addToParent(std::shared_ptr<CustomControlImpl> const & controlImpl);

	void invalidate();

	void invalidateIfVisible();
public:
	virtual ~CustomControl() = 0;

	virtual CustomControlImpl & controlImpl() = 0;

	virtual CustomControlImpl const & controlImpl() const = 0;

	int x() const;

	int y() const;

	Point pos() const;

	int width() const;

	int height() const;

	WDims dims() const;

	Rect rect() const;

	void setPos(Point pos);

	void setDims(WDims dims);

	void setRect(Rect rect);

	void show();

	void hide();

	bool visible() const;

	virtual void setText(NativeString const & text) = 0;

	virtual NativeString text() const = 0;
};

}

#endif
