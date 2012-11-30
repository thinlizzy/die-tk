#ifndef IMAGELIST_WIN32_H_jffffkjfvkvkc0328cbadbhwe54
#define IMAGELIST_WIN32_H_jffffkjfvkvkc0328cbadbhwe54

#include "../../components/ImageList.h"
#include <windows.h>
#include <commctrl.h>
#include "../CommonControlInitializer.h"

namespace tk {

class ImageListImpl: public ImageList, private CommonControlInitializer<0> {
    HIMAGELIST himl;
    int images;
public:
    ImageListImpl(ImageListImpl const &) = delete;

    ImageListImpl(WDims dims, int capacity);
    virtual ~ImageListImpl();

    virtual Index add(ImageHolder ih);
    virtual void replace(ImageHolder ih, Index index);
    virtual void remove(Index index);

    virtual WDims dims() const;
    virtual int count() const;

    virtual void clear();
};

}


#endif
