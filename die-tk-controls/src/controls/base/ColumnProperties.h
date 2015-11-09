#ifndef COLUMN_PROPERTIES_H_fjiij4fi4jcxzmn3
#define COLUMN_PROPERTIES_H_fjiij4fi4jcxzmn3

#include "../../components/ImageList.h"
#include "../../../../src/die-tk.h"

namespace tk {

class ColumnProperties {
public:
    enum Alignment { left, right, center, };
    
    NativeString header;
    int width;
    ImageList::Index imageIndex;
    Alignment alignment;
    
    ColumnProperties(): width(50), imageIndex(ImageList::noIndex), alignment(left) {}
    ColumnProperties(NativeString const & header, int width = 50, ImageList::Index imageIndex = ImageList::noIndex): 
        header(header), width(width), imageIndex(imageIndex), alignment(left) {}
    ColumnProperties & setHeader(NativeString const & header) { this->header = header; return *this; }
    ColumnProperties & setWidth(int width) { this->width = width; return *this; }
    ColumnProperties & setImageIndex(ImageList::Index imageIndex) { this->imageIndex = imageIndex; return *this; }        
    ColumnProperties & setAlignment(Alignment alignment) { this->alignment = alignment; return *this; }        
};

}

#endif
