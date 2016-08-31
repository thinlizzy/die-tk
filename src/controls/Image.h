#ifndef IMAGE_H_DIE_TK_2016_08
#define IMAGE_H_DIE_TK_2016_08

#include "../Canvas.h"
#include "../Control.h"
#include "../components/Image.h"

namespace tk {

class Image: public Control {
public:
    Image() = default;
    Image(Window & parent, ControlParams const & params = ControlParams());
    Image clone() const;
    
	void setImage(image::Ptr image);

	void setAutosize(bool autosize);
    
    Canvas & beginDraw();
    void endDraw();
};

}

#endif
