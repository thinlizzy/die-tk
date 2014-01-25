#ifndef IMAGE_H_dssssfas3289fk560ftgjdwtsss
#define IMAGE_H_dssssfas3289fk560ftgjdwtsss

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
private:
    using Control::getText;
    using Control::setText;
};

}

#endif
