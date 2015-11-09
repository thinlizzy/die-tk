#ifndef COMBOBOX_H_3432fdk450pdfl231
#define COMBOBOX_H_3432fdk450pdfl231

#include <initializer_list>
#include "../../../src/die-tk.h"

namespace tk {

class ComboBox: public Control {
public:
    ComboBox() = default;
    ComboBox(Window & parent, ControlParams const & params = ControlParams());
    ComboBox clone() const;
    
	void addString(NativeString const & str);
    void addStrings(std::initializer_list<NativeString> strings);
	NativeString getString(int index) const;
	void clearItems();
	int selectedIndex() const;
	int count() const;

	void setVisibleItems(int numItems);
	int visibleItems() const;
    
    HandleOperation onChange(HandleOperation callback);
    // TODO on dropdown open
    // TODO on dropdown close
};

}

#endif
