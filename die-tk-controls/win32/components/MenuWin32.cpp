#define _WIN32_WINNT 0x0500
#define WINVER 0x0500
#include <windows.h>

#include "MenuWin32.h"
#include <memory>
#include <unordered_map>
#include "../../../win32/die-tk-win32.h"

namespace tk {
    
unsigned const menuEndPos = unsigned(-2);
std::shared_ptr<WindowImpl> nullWindow;

// contains the menus that are attached to windows - to make findWindow() work to update menu and attached window state
std::unordered_map<HMENU,std::weak_ptr<WindowImpl>> attachedMenus;

UINT lastId = 0;
std::unordered_map<UINT,HandleOperation> clickMap;

struct ItemData {
    image::BitmapPtr image;  // to hold ownership of menu item images
};
std::unordered_map<UINT,ItemData> menuItemData;

void detachExistingMenu(WindowImpl::Components & components)
{
    for( auto it = components.begin(); it != components.end(); ++it ) {
        auto menuPtr = std::dynamic_pointer_cast<MenuImpl>(*it);
        if( menuPtr ) {
            components.erase(it);
            return;
        }
    }
}

std::shared_ptr<WindowImpl> findWindow(HMENU hMenu)
{
    auto it = attachedMenus.find(hMenu);
    if( it == attachedMenus.end() ) return nullWindow;
    
    auto window = it->second.lock();
    if( ! window ) {
        // defensive code: attached window is expired. remove logical attachment and bail out
        attachedMenus.erase(it);
        return nullWindow;
    }
    
    return window;
}

void removeCallbacks(HMENU hMenu);

void removeCallbacks(HMENU hMenu, unsigned pos)
{
    MENUITEMINFOW mii;
    mii.cbSize = sizeof(MENUITEMINFOW);
    mii.fMask = MIIM_ID | MIIM_SUBMENU;
    if( GetMenuItemInfoW(hMenu,pos,TRUE,&mii) == 0 ) {
        log::error("GetMenuItemInfoW returned zero for hMenu ",hMenu);
    } else {
        clickMap.erase(mii.wID);
        menuItemData.erase(mii.wID);

        if( mii.hSubMenu != NULL ) {
            removeCallbacks(mii.hSubMenu);
        }
    }    
}

void removeCallbacks(HMENU hMenu)
{
    auto count = GetMenuItemCount(hMenu);
    for( int pos = 0; pos < count; ++pos ) {
        removeCallbacks(hMenu,pos);
    }
}

void insertNewItem(HMENU hMenu, MenuItemProperties const & properties, UINT pos)
{
    ++lastId;
    MENUITEMINFOW mii;
    mii.cbSize = sizeof(MENUITEMINFOW);
    mii.fMask = MIIM_ID | MIIM_STATE;
    mii.wID = lastId;
    
    // enable and check
    mii.fState = 0;
    if( properties.checked ) {
        mii.fState |= MFS_CHECKED;
    }
    if( ! properties.enabled ) {
        mii.fState |= MFS_DISABLED;
    }
    
    // add the item on the menu 
    if( ! properties.hasImage() ) {
        mii.fMask |= MIIM_STRING;
        mii.dwTypeData = const_cast<wchar_t *>(properties.text.wstr.c_str());
        if( InsertMenuItemW(hMenu,pos,true,&mii) == 0 ) {
            log::error("InsertMenuItemW returned zero for hMenu ",hMenu);
        }
    } else {
        auto imgImpl = std::dynamic_pointer_cast<image::ImageImpl>(properties.image);
        scoped::Bitmap bmImage(imgImpl->cloneHbitmap());
        mii.fMask |= MIIM_BITMAP;
        mii.hbmpItem = bmImage.get();        
        if( InsertMenuItemW(hMenu,pos,true,&mii) == 0 ) {
            log::error("InsertMenuItemW returned zero for hMenu ",hMenu," while adding image with HBITMAP ",mii.hbmpItem);
        } else {
            menuItemData[lastId].image = std::make_shared<image::Bitmap>(bmImage.release());
        }
    }    
}

unsigned getCount(HMENU subMenu)
{
    int result = GetMenuItemCount(subMenu);
    if( result == -1 ) {
        log::error("GetMenuItemCount returned -1 for subMenu ",subMenu);
        return 0;
    }
    
    return unsigned(result);
}

// MenuImpl

MenuImpl::MenuImpl():
    hMenu(CreateMenu())
{
}

MenuImpl::~MenuImpl()
{
    detach();
    removeCallbacks(hMenu);
    DestroyMenu(hMenu);
    // TODO investigate if we need to manually destroy submenus
}

MenuItem MenuImpl::root()
{
    return MenuItem(*this);
}

optional<LRESULT> MenuImpl::processNotification(UINT message, UINT notification, UINT id, LPARAM lParam)
{
    auto it = clickMap.find(id);
    if( it != clickMap.end() ) {
        it->second();
    }
    return optional<LRESULT>();
}

void MenuImpl::attach(std::shared_ptr<WindowImpl> window)
{
    auto windowDims = window->rect().dims();
    if( SetMenu(window->hWnd,hMenu) == 0 ) {
        log::error("SetMenu returned zero for hWnd ",window->hWnd," with hMenu ",hMenu);
        return;
    }
    
    detachExistingMenu(window->components);
    window->components.insert(shared_from_this());
    attachedMenus[hMenu] = std::weak_ptr<WindowImpl>(window);
    window->setDims(windowDims);
}

void MenuImpl::detach()
{
    auto window = findWindow(hMenu);
    if( window ) {
        auto windowDims = window->rect().dims();
        unregister(window);        
        window->components.erase(shared_from_this()); // remove from the window registered components as well
        window->setDims(windowDims);
    } else {
        unregister(window);        
    }
}

Window MenuImpl::getAttachedWindow()
{
    return Window(findWindow(hMenu));
}

void MenuImpl::unregister()
{
    auto window = findWindow(hMenu);
    unregister(window);
}

WDims MenuImpl::payload()
{
    // TODO font may be an issue. use findWindow(hMenu) if that is the case
    return WDims(0,19);
}

void MenuImpl::unregister(std::shared_ptr<WindowImpl> window)
{
    if( window ) {
        if( SetMenu(window->hWnd,0) == 0 ) {
            log::error("SetMenu returned zero for hWnd ",window->hWnd," while removing a menu");
        }
    }

    attachedMenus.erase(hMenu);
}

// MenuItemImpl

MenuItemImpl::MenuItemImpl(HMENU hMenu, unsigned pos):
    hMenu(hMenu),
    pos(pos)
{
}

MenuItemProperties MenuItemImpl::getProperties() const
{
    MenuItemProperties result;
    
    MENUITEMINFOW mii;
    mii.cbSize = sizeof(MENUITEMINFOW);
    mii.fMask = MIIM_STATE | MIIM_TYPE | MIIM_ID;
    mii.dwTypeData = 0;
    if( GetMenuItemInfoW(hMenu,pos,TRUE,&mii) == 0 ) {
        log::error("GetMenuItemInfoW returned zero for hMenu ",hMenu);
    } else {
        result.checked = (mii.fState & MFS_CHECKED) == MFS_CHECKED;
        result.enabled = (mii.fState & MFS_DISABLED) == 0;
        if( mii.fType == MFT_STRING ) {
            ++mii.cch;
            result.text.wstr.resize(mii.cch);
            mii.dwTypeData = &result.text.wstr[0];
            mii.fMask = MIIM_STRING;
            GetMenuItemInfoW(hMenu,pos,TRUE,&mii);
        } else
        if( mii.fType == MFT_BITMAP ) {
            auto it = menuItemData.find(mii.wID);
            if( it == menuItemData.end() || it->second.image->getHbitmap() != mii.hbmpItem ) {
                log::error("bitmap menu had no image set or different from hbmpItem");
                auto img = std::make_shared<image::Bitmap>(mii.hbmpItem);
                menuItemData[mii.wID].image = img;
                result.image = img;
            } else {
                result.image = it->second.image;
            }
        }
    }
    
    return result;
}

HMENU MenuItemImpl::parentHMenu() const
{
    MENUINFO mi;
    mi.cbSize = sizeof(MENUINFO);
    mi.fMask = MIM_MENUDATA;   
    if( GetMenuInfo(hMenu,&mi) == 0 ) {
        log::error("GetMenuInfo returned zero for hMenu ",hMenu);
        return 0;
    }
    return reinterpret_cast<HMENU>(mi.dwMenuData);
    // TODO try GetParent() as well
}

void MenuItemImpl::updateIfTopLevel()
{
    if( parentHMenu() == 0 ) {
        auto window = findWindow(hMenu);
        if( ! window ) return;

        DrawMenuBar(window->hWnd);
    }
}

void MenuItemImpl::setText(NativeString const & text)
{
    MENUITEMINFOW mii;
    mii.cbSize = sizeof(MENUITEMINFOW);
    mii.fMask = MIIM_STRING;
    mii.dwTypeData = const_cast<wchar_t *>(text.wstr.c_str());
    if( SetMenuItemInfoW(hMenu,pos,TRUE,&mii) == 0 ) {
        log::error("SetMenuItemInfoW returned zero for hMenu ",hMenu," and pos ",pos," while changing text");
    }    
    updateIfTopLevel();
}

void MenuItemImpl::setImage(image::Ptr img)
{
    auto imgImpl = std::dynamic_pointer_cast<image::ImageImpl>(img);
    scoped::Bitmap bmImage(imgImpl->cloneHbitmap());
    MENUITEMINFOW mii;
    mii.cbSize = sizeof(MENUITEMINFOW);
    mii.fMask = MIIM_BITMAP;
    mii.hbmpItem = bmImage.get();
    if( SetMenuItemInfoW(hMenu,pos,TRUE,&mii) == 0 ) {
        log::error("SetMenuItemInfoW returned zero for hMenu ",hMenu," and pos ",pos," while changing bitmap with HBITMAP ",mii.hbmpItem);
    } else {
        menuItemData[getItemId()].image = std::make_shared<image::Bitmap>(bmImage.release());
    }
    updateIfTopLevel();    
}

void MenuItemImpl::setState(bool flag, UINT flagValue)
{
    MENUITEMINFOW mii;
    mii.cbSize = sizeof(MENUITEMINFOW);
    mii.fMask = MIIM_STATE;
    if( GetMenuItemInfoW(hMenu,pos,TRUE,&mii) == 0 ) {
        log::error("GetMenuItemInfoW returned zero for hMenu ",hMenu);
        return;
    } 
    
    if( flag ) {
        mii.fState |= flagValue;
    } else {
        mii.fState &= ~flagValue;
    }
    
    if( SetMenuItemInfoW(hMenu,pos,TRUE,&mii) == 0 ) {
        log::error("SetMenuItemInfoW returned zero for hMenu ",hMenu," and pos ",pos," while setting flags");
    }
    updateIfTopLevel();
}

void MenuItemImpl::setEnabled(bool enabled)
{
    setState(!enabled,MFS_DISABLED);
}

void MenuItemImpl::setChecked(bool checked)
{
    setState(checked,MFS_CHECKED);
}

UINT MenuItemImpl::getItemId() const
{
    UINT result = GetMenuItemID(hMenu,pos);
    if( result == UINT(-1) ) {
        log::error("GetMenuItemID returned -1 for hMenu ",hMenu," and pos ",pos);
    }
    return result;
}

HandleOperation MenuItemImpl::onClick(HandleOperation callback)
{
    auto id = getItemId();
    auto it = clickMap.find(id);
    if( it == clickMap.end() ) {
        if( callback != nullptr ) {
            clickMap.insert(it,std::make_pair(id,callback));
        }
        return nullptr;
    }
    
    auto result = it->second;
    if( callback != nullptr ) {
        it->second = callback;
    } else {
        clickMap.erase(it);
    }
    return result;
}

MenuItemIterator MenuItemImpl::begin()
{
    HMENU subMenu = GetSubMenu(hMenu,pos);
    return MenuItemIterator(new MenuItemIteratorImpl(subMenu,subMenu == NULL ? menuEndPos : 0));
}

MenuItemIterator MenuItemImpl::end()
{
    HMENU subMenu = GetSubMenu(hMenu,pos);
    return MenuItemIterator(new MenuItemIteratorImpl(subMenu,menuEndPos));
}

bool MenuItemImpl::empty() const
{
    return count() == 0;
}

unsigned MenuItemImpl::total() const
{
    return getCount(hMenu);
}

unsigned MenuItemImpl::count() const
{
    HMENU subMenu = GetSubMenu(hMenu,pos);
    if( subMenu == NULL ) return 0;
    
    return getCount(subMenu);
}
    
MenuItemIterator MenuItemImpl::addItem(MenuItemProperties const & properties)
{
    HMENU subMenu = GetSubMenu(hMenu,pos);
    int nItems;
    // if submenu is still not created
    if( subMenu == NULL ) {
        // creates a submenu with dwMenuData = hMenu (the parent)
        subMenu = CreatePopupMenu();
        MENUINFO mi;
        mi.cbSize = sizeof(MENUINFO);
        mi.fMask = MIM_MENUDATA;
        mi.dwMenuData = reinterpret_cast<ULONG_PTR>(hMenu);
        if( SetMenuInfo(subMenu,&mi) == 0 ) {
            log::error("SetMenuInfo returned zero for hMenu ",subMenu);
            return MenuItemIterator();
        }
        // sets the submenu handle on the own item
        MENUITEMINFOW mii;
        mii.cbSize = sizeof(MENUITEMINFOW);
        mii.fMask = MIIM_SUBMENU;
        mii.hSubMenu = subMenu;
        if( SetMenuItemInfoW(hMenu,pos,TRUE,&mii) == 0 ) {
            log::error("SetMenuItemInfoW returned zero for hMenu ",hMenu," and pos ",pos," while adding submenu ",subMenu);
            return MenuItemIterator();
        }
        nItems = 0;
    } else {
        nItems = GetMenuItemCount(subMenu);
    }

    // addNewItem(subMenu,properties);
    insertNewItem(subMenu,properties,nItems);
    
    return MenuItemIterator(new MenuItemIteratorImpl(subMenu,nItems));
}

void MenuItemImpl::eraseItem(MenuItemIterator it)
{
    it.iteratorImpl->deleteItem();
}

void MenuItemImpl::advance() const
{
    ++pos;
    if( pos >= total() ) {
        pos = menuEndPos;
    }
}

void MenuItemImpl::deleteItem()
{
    removeCallbacks(hMenu,pos);
    
    if( DeleteMenu(hMenu,pos,MF_BYPOSITION) == 0 ) {
        log::error("DeleteMenu returned zero for hMenu ",hMenu);
    }
    
    updateIfTopLevel();
}

bool MenuItemImpl::operator==(MenuItemImpl const & item) const
{
    return hMenu == item.hMenu && pos == item.pos;
}

// MenuItemImplRoot

MenuItemImplRoot::MenuItemImplRoot(HMENU hMenu):
    MenuItemImpl(hMenu,menuEndPos)
{
}
    
MenuItemProperties MenuItemImplRoot::getProperties() const
{
    return MenuItemProperties();
}

void MenuItemImplRoot::setText(NativeString const & text)
{
}

void MenuItemImplRoot::setImage(image::Ptr img)
{
}

HandleOperation MenuItemImplRoot::onClick(HandleOperation callback)
{
    return nullptr;
}

MenuItemIterator MenuItemImplRoot::begin()
{
    return MenuItemIterator(new MenuItemIteratorImpl(hMenu,0));
}

MenuItemIterator MenuItemImplRoot::end()
{
    return MenuItemIterator(new MenuItemIteratorImpl(hMenu,menuEndPos));
}

unsigned MenuItemImplRoot::count() const
{
    return getCount(hMenu);
}

MenuItemIterator MenuItemImplRoot::addItem(MenuItemProperties const & properties)
{
    int nItems = count();
    
    //addNewItem(hMenu,properties);
    insertNewItem(hMenu,properties,nItems);

    updateTopLevel();    
    
    return MenuItemIterator(new MenuItemIteratorImpl(hMenu,nItems));
}

void MenuItemImplRoot::updateTopLevel()
{
    auto window = findWindow(hMenu);
    if( ! window ) return;

    DrawMenuBar(window->hWnd);
}

// MenuItemIteratorImpl

MenuItemIteratorImpl::MenuItemIteratorImpl(HMENU hMenu, unsigned pos):
    item(std::make_shared<MenuItemImpl>(hMenu,pos))
{
}

MenuItem & MenuItemIteratorImpl::currentItem()
{
    return item;
}

MenuItem const & MenuItemIteratorImpl::currentItem() const
{
    return item;
}

void MenuItemIteratorImpl::advance() const
{
    item.itemImpl->advance();
}

void MenuItemIteratorImpl::deleteItem()
{
    item.itemImpl->deleteItem();
}

bool MenuItemIteratorImpl::operator==(MenuItemIteratorImpl const & it) const
{
    return *item.itemImpl == *it.item.itemImpl;
}

}
