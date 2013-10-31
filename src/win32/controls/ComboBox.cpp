#include "../../controls/ComboBox.h"
#include "ComboBoxWin32.h"

#define CONTROL_IMPL ComboBoxImpl
#include "ControlMacros.h"

namespace tk {

CONSTRUCTOR_IMPL(ComboBox);
CLONE_IMPL(ComboBox);

void ComboBox::addStrings(std::initializer_list<die::NativeString> strings)
{
    for( auto & str : strings ) {
        addString(str);
    }
}

void ComboBox::addString(die::NativeString const & str)
{
    IMPL.addString(str);
}

die::NativeString ComboBox::getString(int index) const
{
    return IMPL.getString(index);
}

void ComboBox::clearItems()
{
    IMPL.clearItems();
}

int ComboBox::selectedIndex() const
{
    return IMPL.selectedIndex();
}

int ComboBox::count() const
{
    return IMPL.count();
}

void ComboBox::setVisibleItems(int numItems)
{
    IMPL.setVisibleItems(numItems);
}

int ComboBox::visibleItems() const
{
    return IMPL.visibleItems();
}

HandleOperation ComboBox::onChange(HandleOperation callback)
{
    return IMPL.onChange(callback);
}

}
