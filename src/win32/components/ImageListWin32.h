#ifndef IMAGELIST_WIN32_H_jffffkjfvkvkc0328cbadbhwe54
#define IMAGELIST_WIN32_H_jffffkjfvkvkc0328cbadbhwe54

#include <windows.h>
#include <commctrl.h>
#include "../CommonControlInitializer.h"
#include "../../components/ImageList.h"

namespace tk {

class ImageListImpl: private CommonControlInitializer<0> {
    HIMAGELIST himl;
    int images;
public:
    ImageListImpl(WDims dims, int capacity);
    ~ImageListImpl();

    ImageList::Index add(ImageRef ih);
    void replace(ImageRef ih, ImageList::Index index);
    void remove(ImageList::Index index);

    WDims dims() const;
    int count() const;

    void clear();
    
    HIMAGELIST getHiml() const;
};

}

#endif
