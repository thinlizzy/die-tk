#ifndef COMBOBOX_WIN32_H_d32h8r437tj54665k
#define COMBOBOX_WIN32_H_d32h8r437tj54665k

#include <string>
#include <vector>
#include "../../../win32/die-tk-win32.h"

namespace tk {

class ComboBoxImpl: public NativeControlImpl {
    int editBoxHeight;
    int nVisibleItems;
    std::vector<NativeString> items;
public:
	ComboBoxImpl(HWND parentHwnd, ControlParams const & params);
    virtual ~ComboBoxImpl();

    virtual ComboBoxImpl * clone() const;

	void addString(NativeString const & str);
	NativeString getString(int index) const;
	void clearItems();
	int selectedIndex() const;
	int count() const;

	void setVisibleItems(int numItems);
	int visibleItems() const;

	virtual Rect rect() const;

	virtual void setDims(WDims dims);
	virtual void setRect(Rect rect);
    
    HandleOperation onChange(HandleOperation callback);
    
    virtual optional<LRESULT> processNotification(UINT message, UINT notification, WPARAM wParam, LPARAM lParam);
private:
	int itemsHeight() const;
	void adjustHeight();
};

}

#endif
