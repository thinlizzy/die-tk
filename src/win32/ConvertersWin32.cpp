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

#define MTE(msg,et) case msg: type = et; break

MouseEvent toMouseEvent(UINT message, WPARAM wParam, bool firstEnter)
{
	MouseEvent result = {};
	switch(message) {
	    case WM_MOUSEMOVE:
            result.type = firstEnter ? et_mouseenter : et_mousemove;
            if( wParam & MK_LBUTTON ) {
                result.button = mb_left;
            } else
            if( wParam & MK_RBUTTON ) {
                result.button = mb_right;
            } else
            if( wParam & MK_MBUTTON ) {
                result.button = mb_middle;
            }
            break;
	    case WM_MOUSELEAVE:
            result.type = et_mouseleave;
            break;
        case WM_LBUTTONDOWN:
            result.type = et_mousedown;
            result.button = mb_left;
            break;
        case WM_LBUTTONUP:
            result.type = et_mouseup;
            result.button = mb_left;
            break;
        case WM_RBUTTONDOWN:
            result.type = et_mousedown;
            result.button = mb_right;
            break;
        case WM_RBUTTONUP:
            result.type = et_mouseup;
            result.button = mb_right;
            break;
        case WM_MBUTTONDOWN:
            result.type = et_mousedown;
            result.button = mb_middle;
            break;
        case WM_MBUTTONUP:
            result.type = et_mouseup;
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

KeyEvent toKeyEvent(UINT message, WPARAM wParam)
{
	auto type = KeyEventType();
	switch(message) {
		MTE(WM_KEYDOWN,et_keydown);
		MTE(WM_KEYUP,et_keyup);
	}
	return KeyEvent(type,fromWindowsKey(wParam));
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

