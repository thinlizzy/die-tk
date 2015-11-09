#ifndef IMAGE_H_dssssfas3289fk560ftgjdwtsss
#define IMAGE_H_dssssfas3289fk560ftgjdwtsss

#include "../../../src/die-tk.h"

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
