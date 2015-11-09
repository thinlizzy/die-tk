#ifndef ITEM_PROPERTIES_H_fjiij4fi4jcxzmn3
#define ITEM_PROPERTIES_H_fjiij4fi4jcxzmn3

#include "../../components/ImageList.h"
#include "../../../../src/die-tk.h"

namespace tk {

class ItemProperties {
public:
    NativeString text;
    ImageList::Index imageIndex;
    ItemProperties(): imageIndex(ImageList::noIndex) {}
    ItemProperties(NativeString const & text, ImageList::Index imageIndex = ImageList::noIndex): text(text), imageIndex(imageIndex) {}
    ItemProperties & setText(NativeString const & text) { this->text = text; return *this; }
    ItemProperties & setImageIndex(ImageList::Index imageIndex) { this->imageIndex = imageIndex; return *this; }        
};

}

#endif
