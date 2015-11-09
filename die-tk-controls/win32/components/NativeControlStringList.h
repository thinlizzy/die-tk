#ifndef NATIVE_CONTROL_STRING_LIST_H_J234234U8R349WERFIOSDF23M423N
#define NATIVE_CONTROL_STRING_LIST_H_J234234U8R349WERFIOSDF23M423N

#include "../../src/components/StringList.h"
#include <deque>
#include <utility>
#include "../../../win32/die-tk-win32.h"

namespace tk {

class NativeControlStringListState;
    
class NativeControlStringList: public StringList {
public:
    NativeControlImpl & control;
    mutable NativeControlStringListState * state;
    mutable NativeString bigString;
    mutable std::deque<size_t> positions;
    
    explicit NativeControlStringList(NativeControlImpl & control);
    void update(NativeString const & text);
    virtual void add(NativeString const & text);
    virtual size_t count() const;
    virtual iterator begin();
    virtual iterator end();
    virtual void insert(iterator pos, NativeString const & text);
    virtual void replace(iterator pos, NativeString const & text);
    virtual void erase(iterator pos);
    virtual NativeString at(size_t pos) const;
    
    void updateControlText();
    void reset(NativeString const & text);
    void partialLoad();
    void fullLoad() const;
    void addNewLine(NativeString const & text);
    void actualInsert(StringList::iterator pos, NativeString const & text);
    void actualReplace(iterator pos, NativeString const & text);
    void actualErase(iterator pos);
    NativeString actualAt(size_t pos) const;
};

}

#endif
