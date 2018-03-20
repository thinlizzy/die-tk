#include "../../controls/PaintBox.h"
#include "PaintBoxWin32.h"

#define CONTROL_IMPL PaintBoxImpl
#include "ControlMacros.h"

namespace tk {

CONSTRUCTOR_IMPL(PaintBox);
CLONE_IMPL(PaintBox);

bool PaintBox::transparent() const
{
	return IMPL.transparent();
}

void PaintBox::setTransparentBackground()
{
	IMPL.setTransparentBackground();
}

}
