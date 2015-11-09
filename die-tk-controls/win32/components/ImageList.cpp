#include "ImageListWin32.h"
#include "../../src/components/ImageList.h"
#include "../../../win32/die-tk-win32.h"

namespace tk {
    
ImageList::ImageList(WDims dims, int capacity):
    impl(std::make_shared<ImageListImpl>(dims,capacity))
{
}

ImageList::ImageList(std::shared_ptr<ImageListImpl> impl):
    impl(impl)
{
}

ImageList::~ImageList()
{}

ImageList::Index ImageList::add(image::Ptr img)
{
    return impl->add(img);
}

void ImageList::replace(image::Ptr img, ImageList::Index index)
{
    impl->replace(img,index);
}

void ImageList::remove(ImageList::Index index)
{
    impl->remove(index);
}

void ImageList::clear()
{
    impl->clear();
}

void ImageList::drawInto(Canvas & canvas, Index index, Point pos)
{
    impl->drawInto(static_cast<CanvasImpl &>(canvas),index,pos);
}

WDims ImageList::dims() const
{
    return impl->dims();
}

int ImageList::count() const
{
    return impl->count();
}

}
