#ifndef EVENT_H_f4330glcp282mvg
#define EVENT_H_f4330glcp282mvg

#include "WindowObjects.h"

namespace tk {

enum WindowKey {
	k_NONE, k_ESCAPE, k_SPACE, k_BACK, k_BACKSPACE=k_BACK, k_RETURN, k_ENTER=k_RETURN, k_TAB,
	k_INSERT, k_DELETE, k_HOME, k_END, k_PAGEUP, k_PAGEDOWN, k_PGUP=k_PAGEUP, k_PGDN=k_PAGEDOWN,
	k_UP, k_DOWN, k_LEFT, k_RIGHT, k_SHIFT, k_CONTROL,
	k_A, k_B, k_C, k_D, k_E, k_F, k_G, k_H, k_I, k_J, k_K, k_L, k_M,
	k_N, k_O, k_P, k_Q, k_R, k_S, k_T, k_U, k_V, k_W, k_X, k_Y, k_Z,
};

enum MouseButton {
    mb_left = 1,
    mb_right,
    mb_middle,
};

struct MouseEvent {
	MouseButton button;
	bool controlPressed;
	bool shiftPressed;
};

enum UserEventType {
	et_user0, et_user1, et_user2, et_user3, et_user4, et_user5, et_user6, et_user7, et_user8, et_user9, et_user10,
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

}

#endif
