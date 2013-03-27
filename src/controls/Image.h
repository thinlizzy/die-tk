#ifndef IMAGE_H_dssssfas3289fk560ftgjdwtsss
#define IMAGE_H_dssssfas3289fk560ftgjdwtsss

#include "../Control.h"

namespace tk {

class Image: public Control {
public:
    Image() = default;
    Image(Window & parent, ControlParams const & params = ControlParams());
    
	void setImage(unsigned char const buffer[], void * header);

	void setAutosize(bool autosize);
    
    using Control::onMouse;
};

}

#endif
