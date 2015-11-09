#include "ImageWin32.h"
#include <memory>
#include <functional>
#include "../../../src/die-tk.h"
#include "../../../win32/die-tk-win32.h"

namespace tk {

ImageImpl::ImageImpl(HWND parentHwnd, ControlParams const & params):
	PaintBoxImpl(parentHwnd,params),
	autosize(params.autosize_),
    image(image::nullImage)
{}

ImageImpl * ImageImpl::clone() const
{
    auto result = std::make_unique<ImageImpl>(getParentHandle(),getControlData().autosize(autosize));
    if( hasImage() ) {
        result->setImage(image);
    }
    return result.release();
}

void ImageImpl::setImage(image::Ptr image)
{
    using namespace std;
    using namespace std::placeholders;

    auto imgImpl = dynamic_pointer_cast<image::ImageImpl>(image);
    this->image = image::cloneBitmap(imgImpl);
    
	if( autosize ) {
		setDims(this->image->dims());
	}
    
    // TODO review this when chained events are supported
    onPaint(bind(&ImageImpl::handlePaint,this,_1,_2));
    
    repaint();
}

void ImageImpl::setAutosize(bool autosize)
{
	bool redim = autosize && ! this->autosize;
	this->autosize = autosize;
	if( redim ) {
		setDims(image->dims());
	}
}

Canvas & ImageImpl::beginDraw()
{
    return image->beginDraw();
}

void ImageImpl::endDraw()
{
    image->endDraw();
}

bool ImageImpl::hasImage() const
{
    return image != image::nullImage;
}

void ImageImpl::handlePaint(Canvas & canvas, Rect rect)
{
    // TODO support partial drawing on image via rect argument
	image->drawInto(canvas,Point());
}

}
