#include "ConvertersX11.h"
#include <X11/Xlib.h>
#include <X11/cursorfont.h>
#include <X11/keysym.h>
#include "ResourceManager.h"

#define XES(et) case et: return #et

namespace tk {

std::string xEventToStr(int eventType)
{
	switch(eventType) {
	case 0: return "error?";
	case 1: return "reply?";
	XES(KeyPress);
	XES(KeyRelease);
	XES(ButtonPress);
	XES(ButtonRelease);
	XES(MotionNotify);
	XES(EnterNotify);
	XES(LeaveNotify);
	XES(FocusIn);
	XES(FocusOut);
	XES(KeymapNotify);
	XES(Expose);
	XES(GraphicsExpose);
	XES(NoExpose);
	XES(VisibilityNotify);
	XES(CreateNotify);
	XES(DestroyNotify);
	XES(UnmapNotify);
	XES(MapNotify);
	XES(MapRequest);
	XES(ReparentNotify);
	XES(ConfigureNotify);
	XES(ConfigureRequest);
	XES(GravityNotify);
	XES(ResizeRequest);
	XES(CirculateNotify);
	XES(CirculateRequest);
	XES(PropertyNotify);
	XES(SelectionClear);
	XES(SelectionRequest);
	XES(SelectionNotify);
	XES(ColormapNotify);
	XES(ClientMessage);
	XES(MappingNotify);
	XES(GenericEvent);
	}

	return "unknown event " + std::to_string(eventType);
}

MouseEvent toMouseEvent(XButtonEvent event)
{
	MouseEvent result;

	switch(event.button) {
	case Button1:
		result.button = MouseButton::left;
		break;
	case Button2:
		result.button = MouseButton::middle;
		break;
	case Button3:
		result.button = MouseButton::right;
		break;
	case Button4:
		result.button = MouseButton::extra1;
		break;
	case Button5:
		result.button = MouseButton::extra2;
		break;
	default:
		result.button = {};
	}

	result.controlPressed = event.state & ControlMask;
	result.shiftPressed = event.state & ShiftMask;

	return result;
}

unsigned int toShape(Cursor cursor)
{
	switch(cursor) {
	case Cursor::defaultCursor: return None;
	case Cursor::arrow: return XC_left_ptr;
	case Cursor::wait: return XC_watch;
	case Cursor::smallwait: return XC_clock; // argh
	case Cursor::hand: return XC_hand2;
	case Cursor::edit: return XC_xterm;
	case Cursor::help: return XC_question_arrow;
	case Cursor::cross: return XC_crosshair;
	}
	return None;
}

unsigned long rgb32(RGBColor const & color)
{
	return (color.r << 16) | (color.g << 8) | color.b;
}

WindowKey fromKeySym(KeySym keySym)
{
	switch(keySym) {
		case XK_Escape: return k_ESCAPE;
		case XK_space: return k_SPACE;
		case XK_BackSpace: return k_BACKSPACE;
		case XK_Tab: return k_TAB;
		case XK_Return: return k_RETURN_BIG;
		case XK_KP_Enter: return k_RETURN_SMALL;
		case XK_Insert: return k_INSERT;
		case XK_Delete: return k_DELETE;
		case XK_Home: return k_HOME;
		case XK_End: return k_END;
		case XK_Prior: return k_PAGEUP;
		case XK_Next: return k_PAGEDOWN;
		case XK_Up: return k_UP;
		case XK_Down: return k_DOWN;
		case XK_Left: return k_LEFT;
		case XK_Right: return k_RIGHT;

		case XK_Shift_L: return k_SHIFT_L;
		case XK_Shift_R: return k_SHIFT_R;
		case XK_Control_L: return k_CONTROL_L;
		case XK_Control_R: return k_CONTROL_R;
		case XK_Alt_L: return k_ALT_L;
		case XK_Alt_R: return k_ALT_R;
		case XK_Pause: return k_PAUSE;
		case XK_Sys_Req: return k_PRINT;
		case XK_Caps_Lock: return k_CAPSLOCK;
		case XK_Num_Lock: return k_NUMLOCK;
		case XK_Scroll_Lock: return k_SCROLLLOCK;

		case XK_KP_Insert:
        case XK_KP_0: return k_NUM0;
        case XK_KP_End:
        case XK_KP_1: return k_NUM1;
        case XK_KP_Down:
        case XK_KP_2: return k_NUM2;
        case XK_KP_Page_Down:
        case XK_KP_3: return k_NUM3;
        case XK_KP_Left:
        case XK_KP_4: return k_NUM4;
        case XK_KP_5: return k_NUM5;
        case XK_KP_Right:
        case XK_KP_6: return k_NUM6;
		case XK_KP_Home:
        case XK_KP_7: return k_NUM7;
        case XK_KP_Up:
        case XK_KP_8: return k_NUM8;
        case XK_KP_Page_Up:
        case XK_KP_9: return k_NUM9;
        case XK_KP_Delete:
        case XK_KP_Decimal: return k_DECIMAL;
        case XK_KP_Add: return k_ADD;
        case XK_KP_Subtract: return k_SUBTRACT;
        case XK_KP_Multiply: return k_MULT;
        case XK_KP_Divide: return k_DIVIDE;

        case XK_colon:
        case XK_semicolon: return k_SEMICOLON;
        case XK_less:
        case XK_comma: return k_COMMA;
        case XK_underscore:
        case XK_minus: return k_DASH;
        case XK_greater:
        case XK_period: return k_DOT;
        case XK_plus:
        case XK_equal: return k_EQUALS;
        case XK_question:
        case XK_slash: return k_SLASH;
		case XK_quotedbl:
        case XK_apostrophe: return k_ACUTE;
        case XK_asciitilde:
        case XK_grave: return k_BACUTE;
        case XK_braceleft:
        case XK_bracketleft: return k_LBRACKET;
        case XK_braceright:
        case XK_bracketright: return k_RBRACKET;
        case XK_bar:
        case XK_backslash: return k_BACKSLASH;

        case XK_F1: return k_F1;
        case XK_F2: return k_F2;
        case XK_F3: return k_F3;
        case XK_F4: return k_F4;
        case XK_F5: return k_F5;
        case XK_F6: return k_F6;
        case XK_F7: return k_F7;
        case XK_F8: return k_F8;
        case XK_F9: return k_F9;
        case XK_F10: return k_F10;
        case XK_F11: return k_F11;
        case XK_F12: return k_F12;

        case XK_A:
		case XK_a: return k_A;
        case XK_B:
		case XK_b: return k_B;
        case XK_C:
		case XK_c: return k_C;
        case XK_D:
		case XK_d: return k_D;
        case XK_E:
		case XK_e: return k_E;
        case XK_F:
		case XK_f: return k_F;
        case XK_G:
		case XK_g: return k_G;
        case XK_H:
		case XK_h: return k_H;
        case XK_I:
		case XK_i: return k_I;
        case XK_J:
		case XK_j: return k_J;
        case XK_K:
		case XK_k: return k_K;
        case XK_L:
		case XK_l: return k_L;
        case XK_M:
		case XK_m: return k_M;
        case XK_N:
		case XK_n: return k_N;
        case XK_O:
		case XK_o: return k_O;
        case XK_P:
		case XK_p: return k_P;
        case XK_Q:
		case XK_q: return k_Q;
        case XK_R:
		case XK_r: return k_R;
        case XK_S:
		case XK_s: return k_S;
        case XK_T:
		case XK_t: return k_T;
        case XK_U:
		case XK_u: return k_U;
        case XK_V:
		case XK_v: return k_V;
        case XK_W:
		case XK_w: return k_W;
        case XK_X:
		case XK_x: return k_X;
        case XK_Y:
		case XK_y: return k_Y;
        case XK_Z:
		case XK_z: return k_Z;

		case XK_exclam:
		case XK_1: return k_1;
		case XK_at:
		case XK_2: return k_2;
		case XK_numbersign:
		case XK_3: return k_3;
		case XK_dollar:
		case XK_4: return k_4;
		case XK_percent:
		case XK_5: return k_5;
		case XK_asciicircum:
		case XK_6: return k_6;
		case XK_ampersand:
		case XK_7: return k_7;
		case XK_asterisk:
		case XK_8: return k_8;
		case XK_parenleft:
		case XK_9: return k_9;
		case XK_parenright:
		case XK_0: return k_0;

		default: return k_NONE;
	}
}

KeySym toKeySym(WindowKey key)
{
	switch(key) {
		case k_ESCAPE: return XK_Escape;
		case k_SPACE: return XK_space;
		case k_BACKSPACE: return XK_BackSpace;
		case k_RETURN_BIG: return XK_Return;
		case k_RETURN_SMALL: return XK_KP_Enter;
		case k_INSERT: return XK_Insert;
		case k_DELETE: return XK_Delete;
		case k_HOME: return XK_Home;
		case k_END: return XK_End;
		case k_PAGEUP: return XK_Prior;
		case k_PAGEDOWN: return XK_Next;
		case k_UP: return XK_Up;
		case k_DOWN: return XK_Down;
		case k_LEFT: return XK_Left;
		case k_RIGHT: return XK_Right;

		case k_SHIFT_L: return XK_Shift_L;
		case k_SHIFT_R: return XK_Shift_R;
		case k_CONTROL_L: return XK_Control_L;
		case k_CONTROL_R: return XK_Control_R;
		case k_ALT_L: return XK_Alt_L;
		case k_ALT_R: return XK_Alt_R;
		case k_PAUSE: return XK_Pause;
		case k_PRINT: return XK_Sys_Req;
		case k_CAPSLOCK: return XK_Caps_Lock;
		case k_NUMLOCK: return XK_Num_Lock;
		case k_SCROLLLOCK: return XK_Scroll_Lock;

        case k_NUM0: return XK_KP_0;
        case k_NUM1: return XK_KP_1;
        case k_NUM2: return XK_KP_2;
        case k_NUM3: return XK_KP_3;
        case k_NUM4: return XK_KP_4;
        case k_NUM5: return XK_KP_5;
        case k_NUM6: return XK_KP_6;
        case k_NUM7: return XK_KP_7;
        case k_NUM8: return XK_KP_8;
        case k_NUM9: return XK_KP_9;
        case k_DECIMAL: return XK_KP_Decimal;
        case k_ADD: return XK_KP_Add;
        case k_SUBTRACT: return XK_KP_Subtract;
        case k_MULT: return XK_KP_Multiply;
        case k_DIVIDE: return XK_KP_Divide;

        case k_SEMICOLON: return XK_semicolon;
        case k_COMMA: return XK_comma;
        case k_DASH: return XK_minus;
        case k_DOT: return XK_period;
        case k_EQUALS: return XK_equal;
        case k_SLASH: return XK_slash;
        case k_ACUTE: return XK_apostrophe;
        case k_BACUTE: return XK_grave;
        case k_LBRACKET: return XK_bracketleft;
        case k_RBRACKET: return XK_bracketright;
        case k_BACKSLASH: return XK_backslash;

        case k_F1: return XK_F1;
        case k_F2: return XK_F2;
        case k_F3: return XK_F3;
        case k_F4: return XK_F4;
        case k_F5: return XK_F5;
        case k_F6: return XK_F6;
        case k_F7: return XK_F7;
        case k_F8: return XK_F8;
        case k_F9: return XK_F9;
        case k_F10: return XK_F10;
        case k_F11: return XK_F11;
        case k_F12: return XK_F12;

		case k_A: return XK_A;
		case k_B: return XK_B;
		case k_C: return XK_C;
		case k_D: return XK_D;
		case k_E: return XK_E;
		case k_F: return XK_F;
		case k_G: return XK_G;
		case k_H: return XK_H;
		case k_I: return XK_I;
		case k_J: return XK_J;
		case k_K: return XK_K;
		case k_L: return XK_L;
		case k_M: return XK_M;
		case k_N: return XK_N;
		case k_O: return XK_O;
		case k_P: return XK_P;
		case k_Q: return XK_Q;
		case k_R: return XK_R;
		case k_S: return XK_S;
		case k_T: return XK_T;
		case k_U: return XK_U;
		case k_V: return XK_V;
		case k_W: return XK_W;
		case k_X: return XK_X;
		case k_Y: return XK_Y;
		case k_Z: return XK_Z;
		case k_1: return XK_1;
		case k_2: return XK_2;
		case k_3: return XK_3;
		case k_4: return XK_4;
		case k_5: return XK_5;
		case k_6: return XK_6;
		case k_7: return XK_7;
		case k_8: return XK_8;
		case k_9: return XK_9;
		case k_0: return XK_0;

		default: return 0;
	}
}

unsigned int toKeyCode(WindowKey key)
{
	auto keySym = toKeySym(key);
	return XKeysymToKeycode(resourceManager.dpy,keySym);
}

}
