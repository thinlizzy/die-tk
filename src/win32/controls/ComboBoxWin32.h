#ifndef COMBOBOX_WIN32_H_d32h8r437tj54665k
#define COMBOBOX_WIN32_H_d32h8r437tj54665k

#include "NativeControlWin32.h"
#include "../../controls/ComboBox.h"

#include <string>
#include <vector>

#pragma warning( push )
#pragma warning( disable : 4250 )

namespace tk {

class ComboBoxImpl: public NativeControlImpl, public ComboBox {
    int editBoxHeight;
    int nVisibleItems;
	std::vector<std::string> items;
public:
	ComboBoxImpl(HWND parent_hWnd, ControlParams const & params);

	virtual void addString(std::string const & str);
	virtual std::string getString(int index) const;
	virtual void clear();
	virtual int selectedIndex() const;
	virtual int count() const;

	virtual void setVisibleItems(int numItems);
	virtual int visibleItems() const;

	virtual Rect rect() const;

	virtual void setDims(WDims dims);
	virtual void setRect(Rect rect);
private:
	int itemsHeight() const;
	void adjustHeight();
};

}

#pragma warning( pop )

#endif
