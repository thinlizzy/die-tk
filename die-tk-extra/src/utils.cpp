#include "utils.h"

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

void drawImage(Canvas & canvas, img::Image const & image, Point start)
{
    canvas.drawImage(ImageRef::native(image.getWindowSystemHeader(),image.rawBits()),start);
}

}