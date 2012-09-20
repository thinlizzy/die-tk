#ifndef COMBOBOX_H_3432fdk450pdfl231
#define COMBOBOX_H_3432fdk450pdfl231

#include "Control.h"
#include <string>

namespace tk {

class ComboBox: public virtual Control {
public:
	virtual void addString(std::string const & str) = 0;
	virtual std::string getString(int index) const = 0;
	virtual void clear() = 0;
	virtual int selectedIndex() const = 0;
	virtual int count() const = 0;

	virtual void setVisibleItems(int numItems) = 0;
	virtual int visibleItems() const = 0;
};

}

#endif
