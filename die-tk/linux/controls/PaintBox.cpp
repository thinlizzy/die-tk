#include "../../controls/PaintBox.h"
#include "PaintBoxX11.h"
#include "../ResourceManager.h"
#include "../WindowImplX11.h"
#include "../../Window.h"

namespace tk {

PaintBox::PaintBox(Window & parent, ControlParams const & params)
{
	impl = std::make_shared<PaintBoxX11>(parent.getImpl().getWindowId(),params);
	parent.getImpl().registerControl(impl);
}

PaintBox PaintBox::clone() const
{
	PaintBox result;
	auto && ci = static_cast<PaintBoxX11 const &>(*impl);
	result.impl = std::shared_ptr<PaintBoxX11>(ci.clone());
	ResourceManagerSingleton resourceManager;
	resourceManager->registerClonedControl(impl);
	return result;
}

bool PaintBox::transparent() const
{
	auto && ci = static_cast<PaintBoxX11 const &>(*impl);
	return ci.transparent();
}

void PaintBox::setTransparentBackground()
{
	auto && ci = static_cast<PaintBoxX11 &>(*impl);
	ci.setTransparentBackground();
}

}
