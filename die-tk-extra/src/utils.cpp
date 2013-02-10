#include "utils.h"
#include "../../../libimage/src/image.h"

namespace tk {
    
ImageList::Index addFile(ImageList & imageList, char const * filename)
{
    img::Image image(filename);
    return imageList.add(ImageRef::native(image.getWindowSystemHeader(),image.rawBits()));
}

}