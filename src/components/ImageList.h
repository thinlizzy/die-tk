#ifndef IMAGE_LIST_H_l4038vmsh3487gk
#define IMAGE_LIST_H_l4038vmsh3487gk

#include <memory>
#include "../WindowObjects.h"

namespace tk {

class ImageListImpl;
    
class ImageList {
public:
    typedef int Index;
    static Index const noIndex;

    std::shared_ptr<ImageListImpl> impl;                // internal use only
    
    explicit ImageList(WDims dims, int capacity = 10);
    ImageList(std::shared_ptr<ImageListImpl> impl);     // internal use only
    ~ImageList();
    
    Index add(ImageRef ih);
    void replace(ImageRef ih, Index index);
    void remove(Index index);

    WDims dims() const;
    int count() const;

    void clear();
};

}

#endif
