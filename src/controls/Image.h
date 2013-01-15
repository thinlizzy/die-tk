#ifndef IMAGE_H_dssssfas3289fk560ftgjdwtsss
#define IMAGE_H_dssssfas3289fk560ftgjdwtsss

#include "Control.h"

namespace tk {

class Image: public virtual Control {
public:
	virtual void setImage(unsigned char const buffer[], void * header) = 0;

	virtual void setAutosize(bool autosize) = 0;
    
    using Control::onMouse;
};

}

#endif
