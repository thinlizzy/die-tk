#ifndef IMAGELIST_WIN32_H_jffffkjfvkvkc0328cbadbhwe54
#define IMAGELIST_WIN32_H_jffffkjfvkvkc0328cbadbhwe54

#include <windows.h>
#include <commctrl.h>
#include "../CommonControlInitializer.h"
#include "../../src/components/ImageList.h"

namespace tk {
    
class CanvasImpl;

class ImageListImpl: private CommonControlInitializer<0> {
    HIMAGELIST himl;
    int images;
public:
    ImageListImpl(WDims dims, int capacity);
    ~ImageListImpl();

    ImageList::Index add(image::Ptr img);
    void replace(image::Ptr img, ImageList::Index index);
    void remove(ImageList::Index index);
    void clear();

    void drawInto(CanvasImpl & canvas, ImageList::Index index, Point pos);

    WDims dims() const;
    int count() const;

    HIMAGELIST getHiml() const;
};

}

#endif
