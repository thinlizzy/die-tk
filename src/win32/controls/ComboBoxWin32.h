#ifndef COMBOBOX_WIN32_H_d32h8r437tj54665k
#define COMBOBOX_WIN32_H_d32h8r437tj54665k

#include "NativeControlWin32.h"

#include <string>
#include <vector>

namespace tk {

class ComboBoxImpl: public NativeControlImpl {
    int editBoxHeight;
    int nVisibleItems;
    std::vector<die::NativeString> items;
public:
	ComboBoxImpl(Window & parent, ControlParams const & params);
    virtual ~ComboBoxImpl();

	void addString(die::NativeString const & str);
	die::NativeString getString(int index) const;
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
