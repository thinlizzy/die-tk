#ifndef COMBOBOX_H_3432fdk450pdfl231
#define COMBOBOX_H_3432fdk450pdfl231

#include "../Control.h"
#include <string>
#include <initializer_list>

namespace tk {

class ComboBox: public Control {
public:
    ComboBox() = default;
    ComboBox(Window & parent, ControlParams const & params = ControlParams());
    
	void addString(std::string const & str);
    void addStrings(std::initializer_list<std::string> strings);
	std::string getString(int index) const;
	void clearItems();
	int selectedIndex() const;
	int count() const;

	void setVisibleItems(int numItems);
	int visibleItems() const;
    
    using Control::onMouse;
    HandleOperation onChange(HandleOperation callback);
    // TODO on dropdown open
    // TODO on dropdown close
};

}

#endif
