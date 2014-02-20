#include "utils.h"

namespace tk {
    
ImageList::Index addFile(ImageList & imageList, char const * filename)
{
    img::Image image(filename);
    return imageList.add(toImageExt(image));
}

ImageList::Index addFile(ImageList & imageList, std::string const & filename)
{
    return addFile(imageList,filename.c_str());
}

image::Ptr toImage(img::Image const & image)
{
    return image::create(image::Params(image.getWindowSystemHeader(),image.rawBits()).transparentIndex(image.getTransparentColorIndex()));
}

image::Ptr toImageExt(img::Image const & image)
{
    return image::create(image::Params(image.getWindowSystemHeader(),image.rawBits()).externalBuffer().forceNoTransparent());
}

}