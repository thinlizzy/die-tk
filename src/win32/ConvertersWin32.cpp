#include "ConvertersWin32.h"

#include <sstream>
#include <stdexcept>

namespace tk {

// used in place of to_string since compilers suck
template<typename T>
std::string tos(T v)
{
    std::ostringstream ss;
    ss << v;
    return ss.str();
}

Rect convertRect(RECT const & rect)
{
	return Rect(rect.left, rect.top, rect.right, rect.bottom);
}

RECT convertRect(Rect const & rect)
{
	RECT result = { rect.left, rect.top, rect.right, rect.bottom };
	return result;
}

RECT convertOpenRect(Rect const & rect)
{
	RECT result = { rect.left, rect.top, rect.right+1, rect.bottom+1 };
	return result;
}

Point convertPoint(POINT const & point)
{
    return Point(point.x,point.y);
}

POINT convertPoint(Point const & point)
{
    POINT p = {point.x,point.y};
    return p;
}

WDims sizeToWDims(SIZE size)
{
    return WDims(size.cx,size.cy);
}

WDims lParamToWDims(LPARAM lParam)
{
	return WDims(LOWORD(lParam),HIWORD(lParam));
}

LPARAM WDimsToLParam(WDims dims)
{
	LPARAM result = 0;
	result = dims.width | (dims.height << 16);
	return result;
}

Point lParamToPoint(LPARAM lParam)
{
	POINTS p = MAKEPOINTS(lParam);
	return Point(p.x,p.y);
}

WindowKey fromWindowsKey(WPARAM wParam)
{
	switch(wParam) {
		case VK_ESCAPE: return k_ESCAPE;
		case VK_SPACE: return k_SPACE;
		case VK_BACK: return k_BACKSPACE;
		case VK_TAB: return k_TAB;
		case VK_RETURN: return k_RETURN;
		case VK_INSERT: return k_INSERT;
		case VK_DELETE: return k_DELETE;
		case VK_HOME: return k_HOME;
		case VK_END: return k_END;
		case VK_PRIOR: return k_PAGEUP;
		case VK_NEXT: return k_PAGEDOWN;
		case VK_UP: return k_UP;
		case VK_DOWN: return k_DOWN;
		case VK_LEFT: return k_LEFT;
		case VK_RIGHT: return k_RIGHT;
        
		case VK_SHIFT: return k_SHIFT;
		case VK_CONTROL: return k_CONTROL;        
		case VK_MENU: return k_ALT;
		case VK_PAUSE: return k_PAUSE;
		case VK_SNAPSHOT: return k_PRINT;
		case VK_CAPITAL: return k_CAPSLOCK;
		case VK_NUMLOCK: return k_NUMLOCK;
		case VK_SCROLL: return k_SCROLLLOCK;
        
        case VK_NUMPAD0: k_NUM0;
        case VK_NUMPAD1: k_NUM1;
        case VK_NUMPAD2: k_NUM2;
        case VK_NUMPAD3: k_NUM3;
        case VK_NUMPAD4: k_NUM4;
        case VK_NUMPAD5: k_NUM5;
        case VK_NUMPAD6: k_NUM6;
        case VK_NUMPAD7: k_NUM7;
        case VK_NUMPAD8: k_NUM8;
        case VK_NUMPAD9: k_NUM9;
        case VK_DECIMAL: k_DECIMAL;
        case VK_ADD: k_ADD;
        case VK_SUBTRACT: k_SUBTRACT;
        case VK_MULTIPLY: k_MULT;
        case VK_DIVIDE: k_DIVIDE;
        
        case VK_OEM_1: k_SEMICOLON;
        case VK_OEM_COMMA: k_COMMA;
        case VK_OEM_MINUS: k_DASH;
        case VK_OEM_PERIOD: k_DOT;
        case VK_OEM_PLUS: k_EQUALS;
        case VK_OEM_2: k_SLASH;
        case VK_OEM_7: k_ACUTE;
        case VK_OEM_3: k_BACUTE;
        case VK_OEM_4: k_LBRACKET;
        case VK_OEM_6: k_RBRACKET;
        case VK_OEM_5: k_BACKSLASH;
        
        case VK_F1: return k_F1;
        case VK_F2: return k_F2;
        case VK_F3: return k_F3;
        case VK_F4: return k_F4;
        case VK_F5: return k_F5;
        case VK_F6: return k_F6;
        case VK_F7: return k_F7;
        case VK_F8: return k_F8;
        case VK_F9: return k_F9;
        case VK_F10: return k_F10;
        case VK_F11: return k_F11;
        case VK_F12: return k_F12;

		case 'A': return k_A;
		case 'B': return k_B;
		case 'C': return k_C;
		case 'D': return k_D;
		case 'E': return k_E;
		case 'F': return k_F;
		case 'G': return k_G;
		case 'H': return k_H;
		case 'I': return k_I;
		case 'J': return k_J;
		case 'K': return k_K;
		case 'L': return k_L;
		case 'M': return k_M;
		case 'N': return k_N;
		case 'O': return k_O;
		case 'P': return k_P;
		case 'Q': return k_Q;
		case 'R': return k_R;
		case 'S': return k_S;
		case 'T': return k_T;
		case 'U': return k_U;
		case 'V': return k_V;
		case 'W': return k_W;
		case 'X': return k_X;
		case 'Y': return k_Y;
		case 'Z': return k_Z;
		case '1': return k_1;
		case '2': return k_2;
		case '3': return k_3;
		case '4': return k_4;
		case '5': return k_5;
		case '6': return k_6;
		case '7': return k_7;
		case '8': return k_8;
		case '9': return k_9;
		case '0': return k_0;

		default: return k_NONE;
	}
}

WPARAM toWindowsKey(WindowKey key)
{
	switch(key) {
		case k_ESCAPE: return VK_ESCAPE;
		case k_SPACE: return VK_SPACE;
		case k_BACKSPACE: return VK_BACK;
		case k_ENTER: return VK_RETURN;
		case k_INSERT: return VK_INSERT;
		case k_DELETE: return VK_DELETE;
		case k_HOME: return VK_HOME;
		case k_END: return VK_END;
		case k_PAGEUP: return VK_PRIOR;
		case k_PAGEDOWN: return VK_NEXT;
		case k_UP: return VK_UP;
		case k_DOWN: return VK_DOWN;
		case k_LEFT: return VK_LEFT;
		case k_RIGHT: return VK_RIGHT;

		case k_SHIFT: return VK_SHIFT;
		case k_CONTROL: return VK_CONTROL;
		case k_ALT: return VK_MENU;
		case k_PAUSE: return VK_PAUSE;
		case k_PRINT: return VK_SNAPSHOT;
		case k_CAPSLOCK: return VK_CAPITAL;
		case k_NUMLOCK: return VK_NUMLOCK;
		case k_SCROLLLOCK: return VK_SCROLL;
        
        case k_NUM0: VK_NUMPAD0;
        case k_NUM1: VK_NUMPAD1;
        case k_NUM2: VK_NUMPAD2;
        case k_NUM3: VK_NUMPAD3;
        case k_NUM4: VK_NUMPAD4;
        case k_NUM5: VK_NUMPAD5;
        case k_NUM6: VK_NUMPAD6;
        case k_NUM7: VK_NUMPAD7;
        case k_NUM8: VK_NUMPAD8;
        case k_NUM9: VK_NUMPAD9;
        case k_DECIMAL: VK_DECIMAL;
        case k_ADD: VK_ADD;
        case k_SUBTRACT: VK_SUBTRACT;
        case k_MULT: VK_MULTIPLY;
        case k_DIVIDE: VK_DIVIDE;
        
        case k_SEMICOLON: VK_OEM_1;
        case k_COMMA: VK_OEM_COMMA;
        case k_DASH: VK_OEM_MINUS;
        case k_DOT: VK_OEM_PERIOD;
        case k_EQUALS: VK_OEM_PLUS;
        case k_SLASH: VK_OEM_2;
        case k_ACUTE: VK_OEM_7;
        case k_BACUTE: VK_OEM_3;
        case k_LBRACKET: VK_OEM_4;
        case k_RBRACKET: VK_OEM_6;
        case k_BACKSLASH: VK_OEM_5;

        case k_F1: return VK_F1;
        case k_F2: return VK_F2;
        case k_F3: return VK_F3;
        case k_F4: return VK_F4;
        case k_F5: return VK_F5;
        case k_F6: return VK_F6;
        case k_F7: return VK_F7;
        case k_F8: return VK_F8;
        case k_F9: return VK_F9;
        case k_F10: return VK_F10;
        case k_F11: return VK_F11;
        case k_F12: return VK_F12;

		case k_A: return 'A';
		case k_B: return 'B';
		case k_C: return 'C';
		case k_D: return 'D';
		case k_E: return 'E';
		case k_F: return 'F';
		case k_G: return 'G';
		case k_H: return 'H';
		case k_I: return 'I';
		case k_J: return 'J';
		case k_K: return 'K';
		case k_L: return 'L';
		case k_M: return 'M';
		case k_N: return 'N';
		case k_O: return 'O';
		case k_P: return 'P';
		case k_Q: return 'Q';
		case k_R: return 'R';
		case k_S: return 'S';
		case k_T: return 'T';
		case k_U: return 'U';
		case k_V: return 'V';
		case k_W: return 'W';
		case k_X: return 'X';
		case k_Y: return 'Y';
		case k_Z: return 'Z';
		case k_1: return '1';
		case k_2: return '2';
		case k_3: return '3';
		case k_4: return '4';
		case k_5: return '5';
		case k_6: return '6';
		case k_7: return '7';
		case k_8: return '8';
		case k_9: return '9';
		case k_0: return '0';

		// review that
		case k_NONE: return -1;
		default: return 0;
	}
}

std::string windowsMessageToString(UINT message)
{
#define CK_MSG(id) case id: return #id
	switch(message) {
		CK_MSG(WM_CREATE);
		CK_MSG(WM_DESTROY);
		CK_MSG(WM_SIZE);
		CK_MSG(WM_CLOSE);
		CK_MSG(WM_QUIT);
		CK_MSG(WM_CHAR);
		CK_MSG(WM_COMMAND);
		CK_MSG(WM_KEYDOWN);
		CK_MSG(WM_KEYUP);
		CK_MSG(WM_MOUSEMOVE);
		CK_MSG(WM_PAINT);

		CK_MSG(WM_DRAWITEM);

		CK_MSG(WM_SETCURSOR);

		CK_MSG(WM_LBUTTONDOWN);
		CK_MSG(WM_LBUTTONUP);
		CK_MSG(WM_NCHITTEST);
		CK_MSG(WM_CTLCOLORBTN);
		CK_MSG(WM_ERASEBKGND);
		CK_MSG(WM_MOVE);
		CK_MSG(WM_MOVING);
		CK_MSG(WM_WINDOWPOSCHANGING);
		CK_MSG(WM_GETMINMAXINFO);
		CK_MSG(WM_NCPAINT);
		CK_MSG(WM_GETTEXT);
		CK_MSG(WM_WINDOWPOSCHANGED);
		CK_MSG(WM_CTLCOLORSTATIC);
		CK_MSG(WM_PARENTNOTIFY);
		CK_MSG(WM_MOUSEACTIVATE);
	default:
		return "UNK[" + tos(message) + "]";
	}
}

UserEvent toUserEvent(UINT message, LPARAM lParam)
{
	return UserEvent(
		static_cast<UserEventType>(et_user0+(message-WM_USER)) ,
		reinterpret_cast<void *>(lParam) );
}

MouseEvent toMouseEvent(UINT message, WPARAM wParam)
{
	MouseEvent result = {};
	switch(message) {
        case WM_LBUTTONDOWN:
        case WM_LBUTTONUP:
            result.button = mb_left;
            break;
        case WM_RBUTTONDOWN:
        case WM_RBUTTONUP:
            result.button = mb_right;
            break;
        case WM_MBUTTONDOWN:
        case WM_MBUTTONUP:
            result.button = mb_middle;
            break;
        default:
            // TODO log that
            break;
	}
    result.controlPressed = wParam & MK_CONTROL;
    result.shiftPressed = wParam & MK_SHIFT;
	return result;
}

COLORREF colorWin(RGBColor const & color)
{
	return RGB(color.r,color.g,color.b);
}

UINT convertTextAlign(HTextAlign hta, VTextAlign vta)
{
	UINT format = 0;
	switch( hta ) {
		case hta_left:
			format |= DT_LEFT;
			break;
		case hta_right:
			format |= DT_RIGHT;
			break;
		case hta_center:
			format |= DT_CENTER;
			break;
	};
	switch( vta ) {
		case vta_top:
			format |= DT_TOP;
			break;
		case vta_bottom:
			format |= DT_BOTTOM;
			format |= DT_SINGLELINE;
			break;
		case vta_center:
			format |= DT_VCENTER;
			format |= DT_SINGLELINE;
			break;
	};
    return format;
}

int convertPenStyle(PenStyle style)
{
    switch(style) {
        case ps_solid: return PS_SOLID;
        case ps_dash: return PS_DASH;
        case ps_dot: return PS_DOT;
        case ps_dashdot: return PS_DASHDOT;
        case ps_dashdotdot: return PS_DASHDOTDOT;
        case ps_invisible: return PS_NULL;
        default: return PS_SOLID;
    }
}

}

