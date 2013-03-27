#include "ImageListWin32.h"
#include "../ConvertersWin32.h"
#include "../ScopedObjects.h"
#include <iostream>

namespace tk {

ImageListImpl::ImageListImpl(WDims dims, int capacity)
{
    himl = ImageList_Create(dims.width,dims.height,ILC_COLOR24,capacity,0);
}

ImageListImpl::~ImageListImpl()
{
    if( ! ImageList_Destroy(himl) ) {
        std::cerr << "failure to destroy imagelist" << std::endl;
    }
}

ImageList::Index ImageListImpl::add(ImageRef ih)
{
    scoped::Bitmap bmImage(ihToBitmap(ih));
    int result = ImageList_Add(himl,bmImage.get(),0);
    if( result == -1 ) {
        // TODO log
    }
    return result;
}

void ImageListImpl::replace(ImageRef ih, ImageList::Index index)
{
    scoped::Bitmap bmImage(ihToBitmap(ih));
    BOOL result = ImageList_Replace(himl,index,bmImage.get(),0);
    if( result == 0 ) {
        // TODO log
    }
}

void ImageListImpl::remove(ImageList::Index index)
{
    BOOL result = ImageList_Remove(himl,index);
    if( result == 0 ) {
        // TODO log
    }
}

WDims ImageListImpl::dims() const
{
    int cx,cy;
    BOOL result = ImageList_GetIconSize(himl,&cx,&cy);
    if( result == 0 ) {
        // TODO log
    }
    return WDims(cx,cy);
}

int ImageListImpl::count() const
{
    return ImageList_GetImageCount(himl);
}

void ImageListImpl::clear()
{
    remove(-1);
}

HIMAGELIST ImageListImpl::getHiml() const
{
    return himl;
}

ImageList::Index const ImageList::noIndex = -1;

}
