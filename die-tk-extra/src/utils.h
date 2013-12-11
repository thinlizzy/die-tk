#ifndef UTILS_H_dfjh2834y3h28fh8fh324r345234
#define	UTILS_H_dfjh2834y3h28fh8fh324r345234

#include "../../die-tk.h"
#include <string>
#include <image.h>

namespace tk {
   
ImageList::Index addFile(ImageList & imageList, char const * filename);
ImageList::Index addFile(ImageList & imageList, std::string const & filename);

void drawImage(Canvas & canvas, img::Image const & image, Point start = Point());

ImageRef toImageRef(img::Image const & image);

}

#endif	/* UTILS_H */

