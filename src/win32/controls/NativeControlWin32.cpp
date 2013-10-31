#define WINVER 0x0500
#include "NativeControlWin32.h"
#include <winuser.h>
#include "../ConvertersWin32.h"
#include "../ResourceManager.h"
#include "../CallbackUtils.h"

#include "../../log.h"
#include "../../trace.h"
#include <stdexcept>

namespace tk {

ControlCallbackMap<HandlePaint> cbPaint;
ControlCallbackMap<HandleMouseEvent> cbMouse;
ControlCallbackMap<ProcessKeyEvent> cbKey;
ControlCallbackMap<ProcessKeypress> cbKeypress;

DWORD scrollbarToWinStyle(Scrollbar sb)
{
    switch(sb) {
        case sb_both: return WS_HSCROLL | WS_VSCROLL;
        case sb_horizontal: return WS_HSCROLL;
        case sb_vertical: return WS_VSCROLL;
        default: return 0;
    }
}

Scrollbar getScrollbarStatus(HWND hWnd)
{
    int const state_system_invisible = 0x8000; // STATE_SYSTEM_INVISIBLE is not defined...
    
    Scrollbar sb = sb_none;
    SCROLLBARINFO sbi;
    sbi.cbSize = sizeof(SCROLLBARINFO);
    if( GetScrollBarInfo(hWnd,OBJID_HSCROLL,&sbi) ) {
        if( (sbi.rgstate[0] & state_system_invisible) == 0 ) {
            sb = sb_horizontal;
        }
    }
    if( GetScrollBarInfo(hWnd,OBJID_VSCROLL,&sbi) ) {
        if( (sbi.rgstate[0] & state_system_invisible) == 0 ) {
            sb = sb == sb_none ? sb_vertical : sb_both;
        }        
    }
    return sb;
}           


NativeControlImpl::NativeControlImpl():
    cursor(cur_default),
    backgroundColor(RGBColor()),
    trackingMouse(false)
{
}

// parent.getImpl().hWnd
NativeControlImpl::NativeControlImpl(HWND parentHwnd, ControlParams const & params, wchar_t const classname[], DWORD style):
    cursor(cur_default),
    backgroundColor(RGBColor()),
    rect_(Rect::closed(params.start_,params.dims_))
{
    hWnd = CreateWindowW(classname, NULL,
        WS_CHILD | WS_CLIPSIBLINGS |
            style | 
            (params.visible_ ? WS_VISIBLE : 0) | 
            scrollbarToWinStyle(params.scrollbar_)
            ,
        params.start_.x, params.start_.y,
        params.dims_.width, params.dims_.height,
        parentHwnd,
        NULL,
        GetModuleHandle(NULL), NULL
    );
    if( hWnd == NULL ) {
        throw std::runtime_error(log::nativeErrorString());
    }
}

NativeControlImpl::~NativeControlImpl()
{
    removeFromCb(this,cbPaint);
    removeFromCb(this,cbMouse);
    removeFromCb(this,cbKey);
    removeFromCb(this,cbKeypress);
}

Canvas & NativeControlImpl::canvas()
{
    canvasImpl.getDC(hWnd);
    return canvasImpl;
}

Rect NativeControlImpl::rect() const
{
    return rect_;
}

void NativeControlImpl::setPos(Point pos)
{
    SetWindowPos(hWnd,0,pos.x,pos.y,0,0,SWP_NOSIZE | SWP_NOZORDER);
    rect_ = rect().move(pos);
}

void NativeControlImpl::setDims(WDims dims)
{
    SetWindowPos(hWnd,0,0,0,dims.width,dims.height,SWP_NOMOVE | SWP_NOZORDER);
    rect_ = rect().resize(dims);
}

void NativeControlImpl::setRect(Rect rect)
{
    rect_ = rect;
    SetWindowPos(hWnd,0,rect.left,rect.top,rect.dims().width,rect.dims().height,SWP_NOZORDER);
}

bool NativeControlImpl::enabled() const
{
    return IsWindowEnabled(hWnd);
}

void NativeControlImpl::enable()
{
    EnableWindow(hWnd,true);
}

void NativeControlImpl::disable()
{
    EnableWindow(hWnd,false);
}

bool NativeControlImpl::visible() const
{
    return IsWindowVisible(hWnd);
}

void NativeControlImpl::show()
{
    ShowWindow(hWnd,SW_SHOW);
}

void NativeControlImpl::hide()
{
    ShowWindow(hWnd,SW_HIDE);
}

void NativeControlImpl::bringToFront()
{
    SetWindowPos(hWnd,HWND_TOP,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
}

void NativeControlImpl::sendToBack()
{
    SetWindowPos(hWnd,HWND_BOTTOM,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
}

void NativeControlImpl::setText(die::NativeString const & text)
{
    SetWindowTextW(hWnd,text.wstr.c_str());
}

die::NativeString NativeControlImpl::getText() const
{
    auto len = GetWindowTextLengthW(hWnd);
    if( len == 0 ) return die::NativeString();

    die::NativeString result;
    result.wstr.resize(len+1,0);
    GetWindowTextW(hWnd,&result.wstr[0],len+1);
    result.wstr.pop_back();
    return result;
}

SIZE NativeControlImpl::getTextDims()
{
    auto sdc = getDC();
    auto text = getText();
    SIZE size;
    GetTextExtentPoint32W(sdc.hdc,text.wstr.data(),text.wstr.length(),&size);
    return size;
}

void NativeControlImpl::repaint()
{
    InvalidateRect(hWnd,0,true);
}

scoped::DC NativeControlImpl::getDC() const
{
    return scoped::DC(hWnd);
}

// TODO call winapi SetCursor() if mouse is hovering the control
void NativeControlImpl::setCursor(Cursor cursor)
{
    this->cursor = cursor;
}

void NativeControlImpl::setBackground(RGBColor const & color)
{
    this->backgroundColor = color;
}

Point NativeControlImpl::screenToClient(Point const & point) const
{
    auto p = convertPoint(point);
    ScreenToClient(hWnd,&p);
    return convertPoint(p);
}

HWND NativeControlImpl::getParentHwnd() const
{
    HWND result = GetParent(hWnd);
    if( result == NULL ) {
        log::error("getParentHwnd() failed for hWnd ",hWnd);
    }
    return result;
}

// TODO add cursor, background color and enabled state
ControlParams NativeControlImpl::getControlData() const
{
    return ControlParams()
            .start(rect().pos())
            .dims(rect().dims())
            .text(getText())
            .visible(visible())
            .scrollbar(getScrollbarStatus(hWnd));
    ;
}


// callbacks & messages

HandleMouseEvent NativeControlImpl::onMouse(HandleMouseEvent callback)
{
    return setCallback(this,cbMouse,callback);
}

ProcessKeyEvent NativeControlImpl::onKey(ProcessKeyEvent callback)
{
    return setCallback(this,cbKey,callback);
}

ProcessKeypress NativeControlImpl::onKeypress(ProcessKeypress callback)
{
    return setCallback(this,cbKeypress,callback);
}

HandlePaint NativeControlImpl::onPaint(HandlePaint callback)
{
    return setCallback(this,cbPaint,callback);
}


#define GETCB(mapname,varname) \
    auto varname = fetchCallback(this,mapname); \
    if( varname == nullptr ) return optional<LRESULT>(); 

optional<LRESULT> NativeControlImpl::processMessage(UINT message, WPARAM & wParam, LPARAM & lParam)
{
    optional<LRESULT> result;
    switch(message) {
        case WM_SETCURSOR:
            if( cursor != cur_default ) {
                result = 0;
                SetCursor(resourceManager.cursors[cursor]);
            }
        break;
        case WM_ERASEBKGND:
            if( backgroundColor != RGBColor() ) {
                result = 0;
                HDC hdc = (HDC) wParam;
                RECT rc;
                GetClientRect(hWnd, &rc);
                scoped::Brush brush(CreateSolidBrush(colorWin(backgroundColor)));
                FillRect(hdc, &rc, brush.get());
            }
        break;
            
        case WM_PAINT: {
            GETCB(cbPaint,on_paint);
            scoped::PaintSection sps(hWnd);
            CanvasImpl canvas(sps.ps.hdc);
            auto rect = convertRect(sps.ps.rcPaint);
            on_paint(canvas,rect);
            return 0;
        } break;

        case WM_KEYDOWN:
        case WM_KEYUP:{
            GETCB(cbKey,on_key);
            auto keyEvent = toKeyEvent(message,wParam);
            WindowKey key = on_key(keyEvent);
            if( key == k_NONE ) return 0;

            wParam = toWindowsKey(key);
        } break;

        case WM_CHAR:{
            GETCB(cbKeypress,on_keypress);
            char key = on_keypress(wParam);
            if( key == 0 ) return 0;

            wParam = key;
        } break;

        case WM_LBUTTONDOWN:
        case WM_LBUTTONUP:
        case WM_RBUTTONDOWN:
        case WM_RBUTTONUP:
        case WM_MBUTTONDOWN:
        case WM_MBUTTONUP:
            return doMouseEvent(message,wParam,lParam,false);
        case WM_MOUSEMOVE:
            return doMouseEvent(message,wParam,lParam,! trackingMouse);
        case WM_MOUSELEAVE:
            trackingMouse = false;
            return doMouseEvent(message,wParam,lParam,false);
    }
    return result;
}

optional<LRESULT> NativeControlImpl::doMouseEvent(UINT message, WPARAM wParam, LPARAM lParam, bool firstEnter)
{
    GETCB(cbMouse,on_mouse);
    if( firstEnter ) {
        // track mouse event to get a mouse leave message
        trackingMouse = true;
        TRACKMOUSEEVENT tme;
        tme.cbSize = sizeof(tme);
        tme.dwFlags = TME_LEAVE;
        tme.hwndTrack = hWnd;
        if( TrackMouseEvent(&tme) == 0 ) {
            log::error("TrackMouseEvent() failed for hWnd ",hWnd);
        }
    }
    auto p = lParamToPoint(lParam);
    auto mouseEvent = toMouseEvent(message,wParam,firstEnter);
    on_mouse(mouseEvent,p);
    return 0;
}

optional<LRESULT> NativeControlImpl::processNotification(UINT message, UINT notification, WPARAM wParam, LPARAM lParam)
{
    return optional<LRESULT>();
}

}
