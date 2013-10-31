#include "NativeControlStringList.h"
#include <algorithm>

namespace tk {

class NativeControlStringListState {
public:
    virtual void update(NativeControlStringList & list, die::NativeString const & text) = 0;
    virtual void add(NativeControlStringList & list, die::NativeString const & text) = 0;
    virtual size_t count(NativeControlStringList const & list) const = 0;
    virtual StringList::iterator begin(NativeControlStringList & list) = 0;
    virtual StringList::iterator end(NativeControlStringList & list) = 0;
    virtual void insert(NativeControlStringList & list, StringList::iterator pos, die::NativeString const & text) = 0;
    virtual void replace(NativeControlStringList & list, StringList::iterator pos, die::NativeString const & text) = 0;
    virtual void erase(NativeControlStringList & list, StringList::iterator pos) = 0;
    virtual die::NativeString at(NativeControlStringList const & list, size_t pos) const = 0;
};

class FullyLoadedState: public NativeControlStringListState {
public:
    virtual void update(NativeControlStringList & list, die::NativeString const & text);
    virtual void add(NativeControlStringList & list, die::NativeString const & text);
    virtual size_t count(NativeControlStringList const & list) const;
    virtual StringList::iterator begin(NativeControlStringList & list);
    virtual StringList::iterator end(NativeControlStringList & list);
    virtual void insert(NativeControlStringList & list, StringList::iterator pos, die::NativeString const & text);
    virtual void replace(NativeControlStringList & list, StringList::iterator pos, die::NativeString const & text);
    virtual void erase(NativeControlStringList & list, StringList::iterator pos);
    virtual die::NativeString at(NativeControlStringList const & list, size_t pos) const;    
};
    
class PartiallyLoadedState: public FullyLoadedState {
public:
    virtual void update(NativeControlStringList & list, die::NativeString const & text);
    virtual size_t count(NativeControlStringList const & list) const;
    virtual StringList::iterator begin(NativeControlStringList & list);
    virtual StringList::iterator end(NativeControlStringList & list);
    virtual void insert(NativeControlStringList & list, StringList::iterator pos, die::NativeString const & text);
    virtual void replace(NativeControlStringList & list, StringList::iterator pos, die::NativeString const & text);
    virtual void erase(NativeControlStringList & list, StringList::iterator pos);
    virtual die::NativeString at(NativeControlStringList const & list, size_t pos) const;    
};
    
class UnloadedState: public PartiallyLoadedState {
public:
    virtual void update(NativeControlStringList & list, die::NativeString const & text);
    virtual void add(NativeControlStringList & list, die::NativeString const & text);
};
    
UnloadedState unloaded;
PartiallyLoadedState partiallyLoaded;
FullyLoadedState fullyLoaded;

// FullyLoadedState

void FullyLoadedState::update(NativeControlStringList & list, die::NativeString const & text)
{
    list.state = &partiallyLoaded;
    list.reset(text);
}

void FullyLoadedState::add(NativeControlStringList & list, die::NativeString const & text)
{
    list.addNewLine(text);
    list.updateControlText();
}

size_t FullyLoadedState::count(NativeControlStringList const & list) const
{
    return list.positions.size();
}

StringList::iterator FullyLoadedState::begin(NativeControlStringList & list) 
{
    return StringList::iterator(&list);
}

StringList::iterator FullyLoadedState::end(NativeControlStringList & list)
{
    return StringList::iterator(&list,list.positions.size());
}

void FullyLoadedState::insert(NativeControlStringList & list, StringList::iterator pos, die::NativeString const & text)
{
    list.actualInsert(pos,text);
}

void FullyLoadedState::replace(NativeControlStringList & list, StringList::iterator pos, die::NativeString const & text)
{
    list.actualReplace(pos,text);    
}

void FullyLoadedState::erase(NativeControlStringList & list, StringList::iterator pos)
{
    list.actualErase(pos);
}

die::NativeString FullyLoadedState::at(NativeControlStringList const & list, size_t pos) const
{
    return list.actualAt(pos);
}

// PartiallyLoadedState

void PartiallyLoadedState::update(NativeControlStringList & list, die::NativeString const & text)
{
    list.reset(text);
}

size_t PartiallyLoadedState::count(NativeControlStringList const & list) const
{
    list.fullLoad();
    return FullyLoadedState::count(list);
}

StringList::iterator PartiallyLoadedState::begin(NativeControlStringList & list) 
{
    list.fullLoad();
    return FullyLoadedState::begin(list);
}

StringList::iterator PartiallyLoadedState::end(NativeControlStringList & list)
{
    list.fullLoad();
    return FullyLoadedState::end(list);    
}

void PartiallyLoadedState::insert(NativeControlStringList & list, StringList::iterator pos, die::NativeString const & text)
{
    list.fullLoad();
    FullyLoadedState::insert(list,pos,text);
}

void PartiallyLoadedState::replace(NativeControlStringList & list, StringList::iterator pos, die::NativeString const & text)
{
    list.fullLoad();
    FullyLoadedState::replace(list,pos,text);
}

void PartiallyLoadedState::erase(NativeControlStringList & list, StringList::iterator pos)
{
    list.fullLoad();
    FullyLoadedState::erase(list,pos);    
}

die::NativeString PartiallyLoadedState::at(NativeControlStringList const & list, size_t pos) const
{
    list.fullLoad();
    return FullyLoadedState::at(list,pos);
}

// UnloadedState

void UnloadedState::update(NativeControlStringList & list, die::NativeString const & text)
{
    // nothing
}

void UnloadedState::add(NativeControlStringList & list, die::NativeString const & text)
{
    list.partialLoad();
    PartiallyLoadedState::add(list,text);
}

// NativeControlStringList

NativeControlStringList::NativeControlStringList(NativeControlImpl & control):
    control(control),
    state(&unloaded)
{
}
    
void NativeControlStringList::update(die::NativeString const & text)
{
    state->update(*this,text);
}

void NativeControlStringList::add(die::NativeString const & text)
{
    state->add(*this,text);
}

size_t NativeControlStringList::count() const
{
    return state->count(*this);
}

auto NativeControlStringList::begin() -> iterator
{
    return state->begin(*this);
}

auto NativeControlStringList::end() -> iterator
{
    return state->end(*this);
}

void NativeControlStringList::insert(iterator pos, die::NativeString const & text)
{
    state->insert(*this,pos,text);
}

void NativeControlStringList::replace(iterator pos, die::NativeString const & text)
{
    state->replace(*this,pos,text);
}

void NativeControlStringList::erase(iterator pos)
{
    state->erase(*this,pos);
}

die::NativeString NativeControlStringList::at(size_t pos) const
{
    return state->at(*this,pos);
}

void NativeControlStringList::updateControlText()
{
    control.setText(bigString);
}

void NativeControlStringList::reset(die::NativeString const & text)
{
    bigString = text;
    positions.clear();
}

void NativeControlStringList::partialLoad()
{
    state = &partiallyLoaded;
    bigString = control.getText();
}

void NativeControlStringList::fullLoad() const
{
    state = &fullyLoaded;
    if( bigString.empty() ) {
        bigString = control.getText();
    }
    
    if( bigString.empty() ) return;
    
    size_t p = 0;
    positions.push_back(p);
    do {
        auto p2 = bigString.wstr.find(L'\n',p);  // TODO delegate this to NativeString due to possible UTF8 issues
        if( p2 == std::string::npos ) break;
        
        p = p2 + 2;
        positions.push_back(p);
    } while( p < bigString.wstr.length() );
}

void NativeControlStringList::addNewLine(die::NativeString const & text)
{
    if( bigString.wstr.empty() ) {
        bigString.wstr = text.wstr;
    } else {
        bigString.wstr += L"\r\n" + text.wstr;
    }
}

void NativeControlStringList::actualInsert(StringList::iterator pos, die::NativeString const & text)
{
    auto pos_it = positions.begin() + pos.getPos();
    if( pos_it == positions.end() ) {
        addNewLine(text);
        positions.push_back(pos.getPos()+2);
    } else {
        auto location = *pos_it;
        bigString.wstr.insert(location,text.wstr+L"\n\r");
        auto newLength = text.wstr.length() + 2;
        std::for_each(pos_it,positions.end(),[newLength](size_t & position) {
            position += newLength;
        });
        positions.insert(pos_it,location);
    }
    
    updateControlText();
}

void NativeControlStringList::actualReplace(StringList::iterator pos, die::NativeString const & text)
{
    auto pos_it = positions.begin() + pos.getPos();
    auto location = *pos_it;
    auto pos_it2 = pos_it + 1;
    if( pos_it2 == positions.end() ) { // last line w/o new line
        auto length = bigString.wstr.length() - location;
        bigString.wstr.replace(location,length,text.wstr);
    } else {
        auto length = *pos_it2 - location;
        bigString.wstr.replace(location,length,text.wstr+L"\n\r");
        auto newLength = text.wstr.length() + 2;
        std::for_each(pos_it2,positions.end(),[newLength,length](size_t & position) {
            position += newLength - length;
        });
    }
    
    updateControlText();
}

void NativeControlStringList::actualErase(StringList::iterator pos)
{
    auto pos_it = positions.begin() + pos.getPos();
    auto location = *pos_it;
    auto pos_it2 = pos_it + 1;
    if( pos_it2 == positions.end() ) { // last line w/o new line
        if( pos_it == positions.begin() ) {
            bigString.wstr.clear();
        } else {
            location -= 2;
            auto length = bigString.wstr.length() - location;
            bigString.wstr.replace(location,length,L"");
        }
    } else {
        auto length = *pos_it2 - location;
        bigString.wstr.replace(location,length,L"");
        std::for_each(pos_it2,positions.end(),[length](size_t & position) {
            position -= length;
        });
    }
    positions.erase(pos_it);
    
    updateControlText();
}

die::NativeString NativeControlStringList::actualAt(size_t pos) const
{
    auto location = positions[pos];
    if( pos == positions.size()-1 ) {
        return bigString.wstr.substr(location);
    } else {
        auto length = positions[pos+1] - location - 2;
        return bigString.wstr.substr(location,length);
    }
}

}
    