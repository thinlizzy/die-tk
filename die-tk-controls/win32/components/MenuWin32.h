#ifndef MENU_WIN32_H_FDFDSFDHUERWE237281FHj39999fu
#define MENU_WIN32_H_FDFDSFDHUERWE237281FHj39999fu

#include <windows.h>
#include "../../src/components/Menu.h"
#include "../../../win32/die-tk-win32.h"

namespace tk {

class MenuImpl: public WindowComponent {
public:
    HMENU hMenu;
    
    MenuImpl();
    ~MenuImpl();
    MenuItem root();
    
    void attach(std::shared_ptr<WindowImpl> window);
    void detach();
    Window getAttachedWindow();
    
    void unregister() override;
    optional<LRESULT> processNotification(UINT message, UINT notification, UINT id, LPARAM lParam) override;
private:
    void unregister(std::shared_ptr<WindowImpl> window);
};

class MenuItemImpl {
public:
    HMENU hMenu;
    mutable unsigned pos;
    
    MenuItemImpl(HMENU hMenu, unsigned pos);
    
    virtual MenuItemProperties getProperties() const;
    virtual void setText(NativeString const & text);
    virtual void setImage(image::Ptr img);

    void setEnabled(bool enabled);
    void setChecked(bool checked);
    
    virtual HandleOperation onClick(HandleOperation callback);
    
    virtual MenuItemIterator begin();
    virtual MenuItemIterator end();
    bool empty() const;
    virtual unsigned count() const;
    
    virtual MenuItemIterator addItem(MenuItemProperties const & properties);
    void eraseItem(MenuItemIterator it);
    
    void advance() const;
    void deleteItem();
    
    bool operator==(MenuItemImpl const & item) const;
private:
    void updateIfTopLevel();
    void setState(bool flag, UINT flagValue);
    HMENU parentHMenu() const;
    UINT getItemId() const;
    unsigned total() const;
};

class MenuItemImplRoot: public MenuItemImpl {
public:
    explicit MenuItemImplRoot(HMENU hMenu);
    
    virtual MenuItemProperties getProperties() const;
    virtual void setText(NativeString const & text);
    virtual void setImage(image::Ptr img);
    
    virtual HandleOperation onClick(HandleOperation callback);
    
    // TODO stubs for enable and check
    
    virtual MenuItemIterator begin();
    virtual MenuItemIterator end();
    virtual unsigned count() const;
    
    virtual MenuItemIterator addItem(MenuItemProperties const & properties);
private:
    void updateTopLevel();
};

class MenuItemIteratorImpl {
    friend class MenuItemIterator;
    friend class MenuItemImpl;
    friend class MenuItemImplRoot;
    
    MenuItem item;
public:
    MenuItemIteratorImpl(HMENU hMenu, unsigned pos);
    
    MenuItem & currentItem();
    
    MenuItem const & currentItem() const;
    
    void advance() const;
    void deleteItem();
    
    bool operator==(MenuItemIteratorImpl const & it) const;
};

}

#endif
