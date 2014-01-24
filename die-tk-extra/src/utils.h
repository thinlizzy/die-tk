#ifndef UTILS_H_dfjh2834y3h28fh8fh324r345234
#define	UTILS_H_dfjh2834y3h28fh8fh324r345234

#include "../../die-tk.h"
#include <string>
#include <image.h>

namespace tk {
   
ImageList::Index addFile(ImageList & imageList, char const * filename);
ImageList::Index addFile(ImageList & imageList, std::string const & filename);

image::Ptr toImage(img::Image const & image);
image::Ptr toImageExt(img::Image const & image); // for quick drawing w/o creating a bitmap

}

#endif	/* UTILS_H */

