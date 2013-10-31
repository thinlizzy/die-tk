#ifndef NATIVE_CONTROL_STRING_LIST_H_J234234U8R349WERFIOSDF23M423N
#define NATIVE_CONTROL_STRING_LIST_H_J234234U8R349WERFIOSDF23M423N

#include "../StringList.h"
#include "controls/NativeControlWin32.h"
#include <deque>
#include <utility>

namespace tk {

class NativeControlStringListState;
    
class NativeControlStringList: public StringList {
public:
    NativeControlImpl & control;
    mutable NativeControlStringListState * state;
    mutable die::NativeString bigString;
    mutable std::deque<size_t> positions;
    
    explicit NativeControlStringList(NativeControlImpl & control);
    void update(die::NativeString const & text);
    virtual void add(die::NativeString const & text);
    virtual size_t count() const;
    virtual iterator begin();
    virtual iterator end();
    virtual void insert(iterator pos, die::NativeString const & text);
    virtual void replace(iterator pos, die::NativeString const & text);
    virtual void erase(iterator pos);
    virtual die::NativeString at(size_t pos) const;
    
    void updateControlText();
    void reset(die::NativeString const & text);
    void partialLoad();
    void fullLoad() const;
    void addNewLine(die::NativeString const & text);
    void actualInsert(StringList::iterator pos, die::NativeString const & text);
    void actualReplace(iterator pos, die::NativeString const & text);
    void actualErase(iterator pos);
    die::NativeString actualAt(size_t pos) const;
};

}

#endif
