#include "../../controls/Image.h"
#include "../../NullCanvas.h"
// #include "ImageX11.h"

namespace tk {

Image::Image(Window & parent, ControlParams const & params)
{
	// TODO delegate to ImageX11
}

Image Image::clone() const
{
	Image result;
	// TODO delegate to ImageX11
	return result;
}

void Image::setImage(image::Ptr image)
{
	// TODO delegate to ImageX11
}

void Image::setAutosize(bool autosize)
{
	// TODO delegate to ImageX11
}

Canvas & Image::beginDraw()
{
	// TODO delegate to ImageX11
	return nullCanvas;
}

void Image::endDraw()
{
	// TODO delegate to ImageX11
}

}
