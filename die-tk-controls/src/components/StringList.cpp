#include "StringList.h"

namespace tk {

StringList::iterator::iterator():
    list(0)
{
}

StringList::iterator::iterator(StringList * list, size_t pos):
    list(list),
    pos(pos)
{
}

NativeString const StringList::iterator::operator*() const
{
    return list->at(pos);
}

StringList::iterator & StringList::iterator::operator++()
{
    ++pos;
    return *this;
}

StringList::iterator StringList::iterator::operator++(int)
{
    iterator result(*this);
    operator++();
    return result;
}

bool StringList::iterator::operator==(StringList::iterator other) const
{
    return &list == &other.list && pos == other.pos;
}

size_t StringList::iterator::getPos() const
{
    return pos;
}

}
