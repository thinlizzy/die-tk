#ifndef EVENT_H_f4330glcp282mvg
#define EVENT_H_f4330glcp282mvg

#include "WindowObjects.h"

namespace tk {

enum UserEventType {
	et_user0, et_user1, et_user2, et_user3, et_user4, et_user5, et_user6, et_user7, et_user8, et_user9, et_user10,
};

enum KeyEventType {
	et_keydown = 1, et_keyup,
};

enum MouseEventType {
    et_mouseenter = 1,
	et_mousemove,
    et_mouseleave,
	et_mousedown, 
    et_mouseup,
};

enum MouseButton {
    mb_left = 1,
    mb_right,
    mb_middle,
};

struct UserEvent {
	UserEventType type;
	union {
		int idata;
		void * data;
	};

	UserEvent(UserEventType type): type(type) {}
	UserEvent(UserEventType type, int idata): type(type), idata(idata) {}
	UserEvent(UserEventType type, void * data): type(type), data(data) {}
};

struct KeyEvent {
	KeyEventType type;
	WindowKey key;

	KeyEvent(KeyEventType type, WindowKey key = k_NONE): type(type), key(key) {}
};

struct MouseEvent {
	MouseEventType type;
	MouseButton button;
	bool controlPressed;
	bool shiftPressed;
};

}

#endif
