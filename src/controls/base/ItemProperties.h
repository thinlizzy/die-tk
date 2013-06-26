#ifndef ITEM_PROPERTIES_H_fjiij4fi4jcxzmn3
#define ITEM_PROPERTIES_H_fjiij4fi4jcxzmn3

#include "../../components/ImageList.h"
#include <NativeString.h>

namespace tk {

class ItemProperties {
public:
    die::NativeString text;
    ImageList::Index imageIndex;
    ItemProperties(): imageIndex(ImageList::noIndex) {}
    ItemProperties(die::NativeString const & text, ImageList::Index imageIndex = ImageList::noIndex): text(text), imageIndex(imageIndex) {}
    ItemProperties & setText(die::NativeString const & text) { this->text = text; return *this; }
    ItemProperties & setImageIndex(ImageList::Index imageIndex) { this->imageIndex = imageIndex; return *this; }        
};

}

#endif
