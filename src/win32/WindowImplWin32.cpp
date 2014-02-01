#include "WindowImplWin32.h"
#include <algorithm>
#include <stdexcept>

#include "ConvertersWin32.h"
#include "ScopedObjects.h"
#include "ResourceManager.h"
#include "CallbackUtils.h"

#include "../log.h"

#include "../trace.h"

namespace {

std::shared_ptr<tk::NativeControlImpl> nullControl;

}

namespace tk {

ControlCallbackMap<AllowOperation> cbClose;
ControlCallbackMap<ProcessResize> cbResize;
ControlCallbackMap<HandleEvent> cbUserEvent;

WindowClass::WindowClass()
{
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = WndProc;   // from ApplicationWin32
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = GetModuleHandle(NULL);
	wc.hIcon = LoadIcon(NULL,IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL,IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = L"die-tk-window";
	RegisterClassW(&wc);
}

WindowClass WindowImpl::windowClass;



tk::WDims windowPayload(8,27);

DWORD stateToWinStyle(int state)
{
	DWORD result = 0;
	if( state & ws_visible ) {
        result |= WS_VISIBLE;
    }
	if( state & ws_minimized ) {
		result |= WS_MINIMIZE;
	} else
	if( state & ws_maximized ) {
		result |= WS_MAXIMIZE;
	}

	return result;
}


WindowImpl::WindowImpl(WindowParams const & params):
	state_(params.initialState)
{
	// create window
	int x,y;
	WDims dims;
	if( params.isDefaultPos() ) {
		x = y = CW_USEDEFAULT;
	} else {
		x = params.start_.x;
		y = params.start_.y;
	}
	if( params.isDefaultDims() ) {
		dims.width = dims.height = CW_USEDEFAULT;
	} else {
		dims = params.dims_ + windowPayload;
	}

	hWnd = createWindow(Point(x,y),dims,params.text_.wstr.c_str(),windowClass.wc.lpszClassName,stateToWinStyle(params.initialState));
}

HWND WindowImpl::createWindow(Point pos, WDims dims, wchar_t const windowname[], wchar_t const classname[], DWORD style)
{
	HWND hWnd = CreateWindowExW(
		WS_EX_APPWINDOW,
		classname, windowname,
//		WS_CLIPCHILDREN | WS_OVERLAPPEDWINDOW | style,
		WS_OVERLAPPEDWINDOW | style,
		pos.x, pos.y,
		dims.width, dims.height,
		NULL, NULL,
		GetModuleHandle(NULL), NULL
	);
	if( hWnd == NULL ) {
		throw std::runtime_error(log::nativeErrorString());
	}
    return hWnd;
}

WindowImpl::~WindowImpl()
{
	if( state_ != ws_destroyed ) {
		// DestroyWindow(hWnd);
		// PostMessage(hWnd,WM_CLOSE,0,0);
		PostMessage(hWnd,WM_DESTROY,0,0);
	}
    removeFromCb(this,cbClose);
    removeFromCb(this,cbResize);
    removeFromCb(this,cbUserEvent);
    for( auto & component : components ) {
        component->unregister();
    }
}

WindowImpl * WindowImpl::clone() const
{
    if( state_ == ws_destroyed ) return 0;
    
    return new WindowImpl(WindowParams()
            .start(rect().pos())
            .dims(rect().dims())
            .text(getText())
            .states(state_)
    );
}

void WindowImpl::setRect(Rect rect)
{
    rect = rect.resize(rect.dims() + windowPayload);
    NativeControlImpl::setRect(rect);	
}

void WindowImpl::setDims(WDims dims)
{
	dims += windowPayload;
    NativeControlImpl::setDims(dims);
}

int WindowImpl::state() const { return state_; }

std::wstring ofnFilters(std::vector<SelectFileParams::Filter> const & filters)
{
    std::wstring result;
    for( auto const & filter : filters ) {
        result += filter.second.empty() ? filter.first.wstr : filter.second.wstr;
        result.push_back(0);
        result += filter.first.wstr;
        result.push_back(0);
    }
    
    result.push_back(0);
    return result;
}

wchar_t * ofnString(die::NativeString const & ns)
{
    return ns.empty() ? NULL : const_cast<wchar_t *>(ns.wstr.c_str());
}

DWORD ofnFlags(SelectFileParams const & params)
{
    DWORD result = 0;
    if( params.showHidden_ ) result |= 0x10000000;  // couldn't find it on mingw    
    return result;
}

WORD ofnLast(SelectFileParams const & params, wchar_t needle)
{
    auto p = params.filename_.wstr.rfind(needle);
    if( p == std::wstring::npos ) return 0;
    
    return p+1;
}

std::vector<die::NativeString> ofnFiles(wchar_t * files)
{
    std::vector<die::NativeString> result;
    wchar_t * end = files;
    while( *end ) { 
       wchar_t * beg = end;
       while( *end ) ++end;
       result.push_back(std::wstring(beg,end));
       ++end;
    }
    
    return result;
}


die::NativeString WindowImpl::selectFile(SelectFileParams const & params)
{
    auto files = selectFiles(params,&GetOpenFileNameW,0);
    if( files.empty() ) return die::NativeString();
    
    return files[0];
}

die::NativeString WindowImpl::selectFileForSave(SelectFileParams const & params)
{
    auto files = selectFiles(params,&GetSaveFileNameW,0);
    if( files.empty() ) return die::NativeString();
    
    return files[0];
}

std::vector<die::NativeString> WindowImpl::selectFiles(SelectFileParams const & params)
{
    return selectFiles(params,&GetOpenFileNameW,OFN_ALLOWMULTISELECT | OFN_EXPLORER);
}

std::vector<die::NativeString> WindowImpl::selectFiles(SelectFileParams const & params, OpenSaveFunc * operation, DWORD flags)
{
    OPENFILENAMEW ofn;
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hWnd;
    std::wstring strFilters;
    if( params.filters_.empty() ) {
        ofn.lpstrFilter = NULL;
    } else {
        strFilters = ofnFilters(params.filters_);
        ofn.lpstrFilter = &strFilters[0];
    }
    ofn.lpstrCustomFilter = NULL;
    ofn.nFilterIndex = 0;
    std::wstring buffer = params.filename_.wstr;
    buffer.resize(8192);
    ofn.lpstrFile = &buffer[0];
    ofn.nMaxFile = buffer.size()-1;
    ofn.lpstrFileTitle = NULL;
    ofn.lpstrInitialDir = ofnString(params.path_);
    ofn.lpstrTitle = ofnString(params.title_);
    ofn.Flags = ofnFlags(params) | flags;
    ofn.nFileOffset = ofnLast(params,L'\\');
    ofn.nFileExtension = ofnLast(params,L'.');
    ofn.lpstrDefExt = NULL;

    BOOL ok = operation(&ofn);
    if( ok == 0 ) return std::vector<die::NativeString>();
    
    return ofnFiles(ofn.lpstrFile);
}

void WindowImpl::show()
{
	state_ |= ws_visible;
	NativeControlImpl::show();
}

void WindowImpl::hide()
{
	state_ &= ~ws_visible;
	NativeControlImpl::hide();
}

std::shared_ptr<NativeControlImpl> WindowImpl::findControl(HWND handle)
{
	auto it = controls.find(handle);
	if( it == controls.end() ) return nullControl;

	return it->second;
}

void WindowImpl::registerControl(std::shared_ptr<NativeControlImpl> control)
{
	controls[control->hWnd] = control;
    resourceManager.registerControl(control);
}

void WindowImpl::unregisterControl(std::shared_ptr<NativeControlImpl> control)
{
    if( controls.erase(control->hWnd) > 0 ) {
        resourceManager.unregisterControl(control);
        DestroyWindow(control->hWnd);
    }
}

// callbacks & messages

AllowOperation WindowImpl::onClose(AllowOperation callback)
{
    return setCallback(this,cbClose,callback);    
}

ProcessResize WindowImpl::onResize(ProcessResize callback)
{
    return setCallback(this,cbResize,callback);    
}

HandleEvent WindowImpl::onUserEvent(HandleEvent callback)
{
    return setCallback(this,cbUserEvent,callback);    
}

optional<LRESULT> WindowImpl::processMessage(UINT message, WPARAM & wParam, LPARAM & lParam)
{
    if( auto result = NativeControlImpl::processMessage(message,wParam,lParam) ) return result;

	switch( message ) {
		case WM_DESTROY:
			state_ = ws_destroyed;
			PostQuitMessage(0);			// TODO really need that? I am not processing WM_QUIT anymore
			break;
            
		case WM_CLOSE: {
            auto canClose = findExec<bool>(this,cbClose);
            if( canClose ) {
                if( *canClose == false ) return 0;
            }
            
			hide();
			return 0;            
		} break;

		case WM_SIZE: {
			WDims newDims = lParamToWDims(lParam);
			switch(wParam) {
				case SIZE_MINIMIZED:
					state_ |= ws_minimized;
					state_ &= ~ws_maximized;
					break;
				case SIZE_MAXIMIZED:
					state_ &= ~ws_minimized;
					state_ |= ws_maximized;
					break;
				case SIZE_RESTORED:
					state_ &= ~(ws_minimized | ws_maximized);
					break;
			}
            
            auto resNewDims = findExec<WDims>(this,cbResize,newDims);
            if( resNewDims ) {
                // TODO verify the need of creating on_minimize and on_maximize callbacks also
                lParam = WDimsToLParam(*resNewDims);
            }            
		} break;
        
		case WM_COMMAND: {
            if( lParam == 0 ) {
                for( auto & component : components ) {
                    auto result = component->processNotification(WM_COMMAND,HIWORD(wParam),LOWORD(wParam),lParam);
                    if( result ) return result;
                }
            } else
			if( auto control = resourceManager.findControl(HWND(lParam)) ) {
                return control->processNotification(WM_COMMAND,HIWORD(wParam),wParam,lParam);
			}
		} break;

		case WM_NOTIFY: {
            auto hdr = reinterpret_cast<LPNMHDR>(lParam);
			if( auto control = resourceManager.findControl(hdr->hwndFrom) ) {
                return control->processNotification(WM_NOTIFY,hdr->code,wParam,lParam);
			}
		} break;

		default:
			if( message >= WM_USER ) {
                if( findExec(this,cbUserEvent,toUserEvent(message,lParam)) ) {
                    return 0;
                }
			}
	}

	return optional<LRESULT>();
}

}
