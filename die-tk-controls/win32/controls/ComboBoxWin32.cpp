#include "ComboBoxWin32.h"
#include <memory>
#include "../../../src/die-tk.h"

namespace tk {

CallbackMap<ComboBoxImpl *, HandleOperation> cbChange;

ControlParams chkComboDefaults(ControlParams params)
{
	if( params.dims_.width == 0 ) {
		params.dims_.width = 100;
	}
	if( params.dims_.height < 25 ) {  // TODO get 25 from font height
		params.dims_.height = 25;
	}
	return params;
}

ComboBoxImpl::ComboBoxImpl(HWND parentHwnd, ControlParams const & params):
    NativeControlImpl(parentHwnd,chkComboDefaults(params),L"combobox",CBS_DROPDOWNLIST | WS_VSCROLL | CBS_HASSTRINGS),
    nVisibleItems(10)
{
    editBoxHeight = params.dims_.height;
    adjustHeight();
}

ComboBoxImpl::~ComboBoxImpl()
{
    removeFromCb(this,cbChange);
}

ComboBoxImpl * ComboBoxImpl::clone() const
{
    auto result = std::make_unique<ComboBoxImpl>(getParentHandle(),getControlData());
    for( auto & item : items ) {
        result->addString(item);
    }
    result->setVisibleItems(visibleItems());
    return result.release();
}

void ComboBoxImpl::addString(NativeString const & str)
{
	bool wasEmpty = items.empty();
	items.push_back(str);
	SendMessageW(hWnd,CB_ADDSTRING,0,reinterpret_cast<LPARAM>(items.back().wstr.c_str()));
	if( wasEmpty ) {
		SendMessageW(hWnd,CB_SETCURSEL,0,0);
	}
}

NativeString ComboBoxImpl::getString(int index) const
{
	return items.at(index);		// TODO wrap exception
}

void ComboBoxImpl::clearItems()
{
	SendMessageW(hWnd,CB_RESETCONTENT,0,0);
	items.clear();
}

int ComboBoxImpl::selectedIndex() const
{
	return SendMessageW(hWnd,CB_GETCURSEL,0,0);
}

int ComboBoxImpl::count() const
{
	return items.size();
}

void ComboBoxImpl::setVisibleItems(int numItems)
{
    nVisibleItems = numItems;
    adjustHeight();
}

int ComboBoxImpl::visibleItems() const
{
    return nVisibleItems;
}

int ComboBoxImpl::itemsHeight() const
{
	scoped::DC sdc(hWnd);
	SIZE size;
	GetTextExtentPoint32W(sdc.hdc,L"|",1,&size);

	float const height_padding = 1;
	return int(size.cy + height_padding) * nVisibleItems;
/*
	float const height_padding = 1; //1.146875;			// TODO: get this value proportional from current font
	return int(size.cy * height_padding) * nVisibleItems;
*/
}

void ComboBoxImpl::adjustHeight()
{
    NativeControlImpl::setDims(NativeControlImpl::rect().dims().setHeight(editBoxHeight + itemsHeight()));
}

Rect ComboBoxImpl::rect() const
{
    Rect res = NativeControlImpl::rect();
    res.bottom -= itemsHeight();
    return res;
}

void ComboBoxImpl::setRect(Rect rect)
{
    editBoxHeight = rect.dims().height;
    rect.bottom = editBoxHeight + itemsHeight();
    NativeControlImpl::setRect(rect);
}

void ComboBoxImpl::setDims(WDims dims)
{
    editBoxHeight = dims.height;
    dims.height = editBoxHeight + itemsHeight();
    NativeControlImpl::setDims(dims);
}

HandleOperation ComboBoxImpl::onChange(HandleOperation callback)
{
    return setCallback(this,cbChange,callback);
}    

optional<LRESULT> ComboBoxImpl::processNotification(UINT message, UINT notification, WPARAM wParam, LPARAM lParam)
{
    if( message == WM_COMMAND && notification == CBN_SELCHANGE ) {
        if( executeCallback(this,cbChange) ) {
            return 0;
        }
	}

    return NativeControlImpl::processNotification(message,notification,wParam,lParam);
}

}
