#include "utils.h"
#include <image.h>

namespace tk {
    
ImageList::Index addFile(ImageList & imageList, char const * filename)
{
    img::Image image(filename);
    return imageList.add(ImageRef::native(image.getWindowSystemHeader(),image.rawBits()));
}

ImageList::Index addFile(ImageList & imageList, std::string const & filename)
{
    return addFile(imageList,filename.c_str());
}

}