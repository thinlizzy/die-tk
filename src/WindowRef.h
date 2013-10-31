#ifndef WINDOW_REFERENCE_H_kf34252943mdvj48dhdrgg
#define WINDOW_REFERENCE_H_kf34252943mdvj48dhdrgg

#include "Window.h"

namespace tk {

class WindowRef: public Window {
public:
    explicit WindowRef(std::shared_ptr<WindowImpl> impl);  // internal use only
    WindowRef(WindowRef const & wr);
    WindowRef & operator=(WindowRef const & wr);
};

}

#endif