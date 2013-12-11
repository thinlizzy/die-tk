#include "utils.h"

namespace tk {
    
ImageList::Index addFile(ImageList & imageList, char const * filename)
{
    img::Image image(filename);
    return imageList.add(toImageRef(image));
}

ImageList::Index addFile(ImageList & imageList, std::string const & filename)
{
    return addFile(imageList,filename.c_str());
}

void drawImage(Canvas & canvas, img::Image const & image, Point start)
{
    canvas.drawImage(toImageRef(image),start);
}

ImageRef toImageRef(img::Image const & image)
{
    return ImageRef::native(image.getWindowSystemHeader(),image.rawBits());
}

}