#ifndef IMAGE_LIST_H_l4038vmsh3487gk
#define IMAGE_LIST_H_l4038vmsh3487gk

#include <memory>
#include "../../../src/die-tk.h"

namespace tk {

class ImageListImpl;
class Canvas;
    
class ImageList {
public:
    typedef int Index;
    static Index const noIndex;

    explicit ImageList(WDims dims, int capacity = 10);
    ~ImageList();
    
    Index add(image::Ptr img);
    void replace(image::Ptr img, Index index);
    void remove(Index index);
    void clear();
    
    void drawInto(Canvas & canvas, Index index, Point pos);

    WDims dims() const;
    int count() const;
    
    // internal use only
    std::shared_ptr<ImageListImpl> impl;
    explicit ImageList(std::shared_ptr<ImageListImpl> impl);    
};

}

#endif
