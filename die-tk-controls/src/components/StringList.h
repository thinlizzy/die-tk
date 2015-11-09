#ifndef STRING_LIST_H_2349R234JUR894HFDER789W345HNGF
#define STRING_LIST_H_2349R234JUR894HFDER789W345HNGF

#include "../../../src/die-tk.h"

namespace tk {

class StringList {
public:
    class iterator {
        StringList * list;
        size_t pos;
    public:
        iterator();
        explicit iterator(StringList * list, size_t pos = 0);
        NativeString const operator*() const;
        iterator & operator++();
        iterator operator++(int);
        bool operator==(iterator other) const;
        size_t getPos() const;
    };
    
    virtual ~StringList() {}
    virtual void add(NativeString const & text) = 0;
    virtual size_t count() const = 0;
    virtual iterator begin() = 0;
    virtual iterator end() = 0;
    virtual void insert(iterator pos, NativeString const & text) = 0;
    virtual void replace(iterator pos, NativeString const & text) = 0;
    virtual void erase(iterator pos) = 0;
    virtual NativeString at(size_t pos) const = 0;
};

}

#endif
