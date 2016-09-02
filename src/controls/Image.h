#ifndef IMAGE_H_DIE_TK_2016_08
#define IMAGE_H_DIE_TK_2016_08

#include "../Canvas.h"
#include "../Control.h"
#include "../components/Image.h"

namespace tk {

/**
 * This control simulates a displayable image buffer which also allows to draw into its canvas.
 * It is still on alpha stage so there are some TODOs:
 * - a better constructor to specify an image upon creation
 * - create empty image if no image is specified
 * - resizing control also needs to resize the image or to increase the canvas size
 * - research support for transparent image (not sure if that works with a drawable canvas. X11 is a challenge too)
 */
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
