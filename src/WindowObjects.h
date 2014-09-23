#ifndef WINDOW_OBJECTS_H_fdsf4394kf2302k4bng
#define WINDOW_OBJECTS_H_fdsf4394kf2302k4bng

namespace tk {

enum WindowState { ws_visible=2, ws_minimized=4, ws_maximized=8, };

enum class Cursor {
    defaultCursor = -1,
    arrow, wait, smallwait, hand, edit, help, cross,
    numCursors,
};

enum class Scrollbar { none, horizontal, vertical, both, };

enum class ClipboardType {
    nothing, text, 
};

}

#endif
