#include "NullImage.h"
#include "../NullCanvas.h"

namespace tk {
namespace image {

Canvas & NullImage::canvas()
{
	return nullCanvas;
}

Ptr nullImage = std::make_shared<NullImage>();

}
}
