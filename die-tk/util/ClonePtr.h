#ifndef CLONE_PTR_H_jgfgrs89fg45j35fdl033o1jfds
#define CLONE_PTR_H_jgfgrs89fg45j35fdl033o1jfds

#include <memory>

template<typename T>
class ClonePtr {
    std::unique_ptr<T> data;
public:
    ClonePtr() = default;
    ClonePtr(ClonePtr &&) = default;
    ~ClonePtr() = default;
    explicit ClonePtr(T * ptr): data(ptr) {}
    ClonePtr(ClonePtr const & ptr): data(ptr.data ? new T(*ptr.data) : 0) {}
    ClonePtr & operator=(ClonePtr const & ptr)
    {
        data.reset(ptr.data ? new T(*ptr.data) : 0);
        return *this;
    }
    T & operator*() { return *data; }
    T const & operator*() const { return *data; }
    T * operator->() { return data.get(); }
    T const * operator->() const { return data.get(); }

    explicit operator bool() const { return data.get(); }
    bool operator!() const { return ! data.get(); }
};

#endif
