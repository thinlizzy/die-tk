#ifndef MENU_H_kd23490fsdggfdl2340324ndgf
#define MENU_H_kd23490fsdggfdl2340324ndgf

#include <NativeString.h>
#include <memory>
#include "../WindowObjects.h"
#include "../Window.h"
#include "../util/ClonePtr.h"
#include "Image.h"

namespace tk {

class MenuItem;
class MenuImpl;
class MenuItemImpl;
class MenuItemIterator;
class MenuItemIteratorImpl;

class Menu {
public:
    Menu();
    ~Menu();
    
    Menu(Menu const &) = delete;
    Menu & operator=(Menu const &) = delete;
    Menu(Menu &&) = default;
    Menu & operator=(Menu &&) = default;
    
    MenuItem root();
    
    void attach(Window & window);
    void detach();
    Window getAttachedWindow();
    
    // internal use only
    std::shared_ptr<MenuImpl> impl;
    explicit Menu(std::shared_ptr<MenuImpl> impl);
};

class MenuItemProperties {
public:
    bool enabled, checked;
    die::NativeString text;
    image::Ptr image;
    
    MenuItemProperties(die::NativeString text = die::NativeString()): 
        enabled(true),
        checked(false),
        text(text),
        image(image::nullImage)
    {}
    
    MenuItemProperties(image::Ptr image): 
        enabled(true),
        checked(false),
        image(image)
    {}
    
    MenuItemProperties & setEnabled(bool value) { enabled = value; return *this; }
    MenuItemProperties & setChecked(bool value) { checked = value; return *this; }
    
    bool hasImage() const { return image != image::nullImage; }
};

class MenuItem {
    friend class MenuImpl;
    friend class MenuItemIteratorImpl;
public:
    MenuItemProperties getProperties() const;
    void setText(die::NativeString const & text);
    void setImage(image::Ptr img);
    
    void enable() { setEnabled(true); }
    void disable() { setEnabled(false); }
    void setEnabled(bool enabled);
    
    void check() { setChecked(true); }
    void uncheck() { setChecked(false); }
    void setChecked(bool checked);
    
    MenuItemIterator begin();
    MenuItemIterator end();
    MenuItemIterator const begin() const;
    MenuItemIterator const end() const;
    bool empty() const;
    
    MenuItemIterator addItem(MenuItemProperties const & properties);
    MenuItemIterator addItem(die::NativeString const & text);
    void eraseItem(MenuItemIterator it);
    
    HandleOperation onClick(HandleOperation callback);
    // TODO owner draw
private:
    std::shared_ptr<MenuItemImpl> itemImpl;
    MenuItem() = default;
    explicit MenuItem(MenuImpl & menuImpl);
    explicit MenuItem(std::shared_ptr<MenuItemImpl> itemImpl);
};

class MenuItemIterator: public std::iterator<std::forward_iterator_tag,MenuItem> {
    friend class MenuItem;
    friend class MenuItemImpl;
    friend class MenuItemImplRoot;
public:
    MenuItemIterator();
    MenuItemIterator(MenuItemIterator const & it);
    MenuItemIterator & operator=(MenuItemIterator const & it);
    ~MenuItemIterator();

    MenuItem & operator*();
    MenuItem const & operator*() const;
    MenuItem * operator->();
    MenuItem const * operator->() const;
    MenuItemIterator & operator++();
    MenuItemIterator const & operator++() const;
    MenuItemIterator operator++(int) const;
    bool operator==(MenuItemIterator const & it) const;
    bool operator!=(MenuItemIterator const & it) const;
    explicit operator bool() const;
private:
    ClonePtr<MenuItemIteratorImpl> iteratorImpl;
    explicit MenuItemIterator(MenuItemIteratorImpl * iteratorImpl);
};

}

#endif
