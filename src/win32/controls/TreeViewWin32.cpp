#include "TreeViewWin32.h"
#include "../components/ImageListWin32.h"
#include "../../CallbackUtils.h"
#include "../../controls/base/ItemProperties.h"
#include <memory>

#ifndef MAX_TREEVIEW_ITEM_CHARS
#define MAX_TREEVIEW_ITEM_CHARS 261
#endif

namespace tk {

template<typename T> using TreeViewCallbackMap = CallbackMap<TreeViewImpl *, T>; 

TreeViewCallbackMap<TreeView::AllowChangeFromTo> cbBeforeChange;
TreeViewCallbackMap<TreeView::HandleItemOperation> cbOnChange;
TreeViewCallbackMap<TreeView::AllowItemChange> cbBeforeExpand;
TreeViewCallbackMap<TreeView::HandleItemOperation> cbOnExpand;
TreeViewCallbackMap<TreeView::AllowItemChange> cbBeforeCollapse;
TreeViewCallbackMap<TreeView::HandleItemOperation> cbOnCollapse;

// find and execute a callback
template<typename CbType>
bool findExecItem(TreeViewCallbackMap<CbType> & callbackMap, TreeViewImpl * treeView, HTREEITEM hItem)
{
    auto callback = fetchCallback(treeView,callbackMap);    
    if( callback == nullptr ) return false;
    
    callback(TreeView::Item(std::make_shared<ItemImpl>(treeView->hWnd,hItem)));
    return true;
}

/* TreeViewImpl */

TreeViewImpl::TreeViewImpl(HWND parentHwnd, ControlParams const & params):
	NativeControlImpl(parentHwnd,params,WC_TREEVIEWW,TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS),
	rootItemImpl(std::make_shared<ItemImpl>(hWnd,TVI_ROOT))
{
}

TreeViewImpl::~TreeViewImpl()
{
    removeFromCb(this,cbBeforeChange);
    removeFromCb(this,cbOnChange);
    removeFromCb(this,cbBeforeExpand);
    removeFromCb(this,cbOnExpand);
    removeFromCb(this,cbBeforeCollapse);
    removeFromCb(this,cbOnCollapse);
}

void cloneChildren(ItemImpl & dest, ItemImpl const & src)
{
    for( ItemImpl item(src.hTreeView,src.firstChild()); 
            item.hItem != NULL; 
            item = ItemImpl(src.hTreeView,item.nextSibling()) ) {
        dest.addChild(item.getProperties());
        ItemImpl destItemImpl(dest.hTreeView,dest.firstChild());
        cloneChildren(destItemImpl,ItemImpl(item.hTreeView,item.firstChild()));
    }
}

TreeViewImpl * TreeViewImpl::clone() const
{
    auto result = std::make_unique<TreeViewImpl>(getParentHandle(),getControlData());    
    result->imageListImpl = imageListImpl;
    result->rootItemImpl = std::make_shared<ItemImpl>(result->hWnd,TVI_ROOT);
    cloneChildren(*result->rootItemImpl,*rootItemImpl);
    return result.release();
}

TreeView::Item TreeViewImpl::root() const
{
    return TreeView::Item(rootItemImpl);
}

size_t TreeViewImpl::total() const
{
    return TreeView_GetCount(hWnd);
}

void TreeViewImpl::setImageList(ImageList & imageList)
{
    TreeView_SetImageList(hWnd, imageList.impl->getHiml(), TVSIL_NORMAL);
    imageListImpl = imageList.impl;
}

void TreeViewImpl::removeImageList()
{
    TreeView_SetImageList(hWnd, NULL, TVSIL_NORMAL);
    imageListImpl.reset();
}

optional<ImageList> TreeViewImpl::getImageList()
{
    if( ! imageListImpl ) return optional<ImageList>();
            
    return ImageList(imageListImpl);
}

TreeView::Iterator TreeViewImpl::selected() const
{
    HTREEITEM hItem = TreeView_GetSelection(hWnd);
    return TreeView::Iterator(new IteratorImpl(hWnd,hItem));
}

TreeView::AllowChangeFromTo TreeViewImpl::beforeChange(TreeView::AllowChangeFromTo callback)
{
    return setCallback(this,cbBeforeChange,callback);
}

TreeView::HandleItemOperation TreeViewImpl::onChange(TreeView::HandleItemOperation callback)
{
    return setCallback(this,cbOnChange,callback);
}

TreeView::AllowItemChange TreeViewImpl::beforeExpand(TreeView::AllowItemChange callback)
{
    return setCallback(this,cbBeforeExpand,callback);
}

TreeView::HandleItemOperation TreeViewImpl::onExpand(TreeView::HandleItemOperation callback)
{
    return setCallback(this,cbOnExpand,callback);
}

TreeView::AllowItemChange TreeViewImpl::beforeCollapse(TreeView::AllowItemChange callback)
{
    return setCallback(this,cbBeforeCollapse,callback);
}

TreeView::HandleItemOperation TreeViewImpl::onCollapse(TreeView::HandleItemOperation callback)
{
    return setCallback(this,cbOnCollapse,callback);
}

optional<LRESULT> TreeViewImpl::processNotification(UINT message, UINT notification, WPARAM wParam, LPARAM lParam)
{
    if( message == WM_NOTIFY ) {
        switch(notification) {
            case TVN_SELCHANGING: {
                auto it = cbBeforeChange.find(this);
                if( it != cbBeforeChange.end() ) {
                    auto pnmtv = reinterpret_cast<LPNMTREEVIEW>(lParam);
                    bool allowChange = it->second(
                        TreeView::Item(std::make_shared<ItemImpl>(hWnd,pnmtv->itemOld.hItem)),
                        TreeView::Item(std::make_shared<ItemImpl>(hWnd,pnmtv->itemNew.hItem))
                    );
                    if( ! allowChange ) return TRUE;
                }
                } break;
            case TVN_SELCHANGED: {
                auto pnmtv = reinterpret_cast<LPNMTREEVIEW>(lParam);
                if( findExecItem(cbOnChange,this,pnmtv->itemNew.hItem) ) return 0;
                } break;
            case TVN_ITEMEXPANDING: {
                auto pnmtv = reinterpret_cast<LPNMTREEVIEW>(lParam);
                switch(pnmtv->action) {
                    case TVE_EXPAND: {
                        auto it = cbBeforeExpand.find(this);
                        if( it != cbBeforeExpand.end() ) {
                            bool allowExpand = it->second(
                                TreeView::Item(std::make_shared<ItemImpl>(hWnd,pnmtv->itemNew.hItem))
                            );
                            if( ! allowExpand ) return TRUE;
                        }
                        } break;
                    case TVE_COLLAPSE:
                        auto it = cbBeforeCollapse.find(this);
                        if( it != cbBeforeCollapse.end() ) {
                            bool allowCollapse = it->second(
                                TreeView::Item(std::make_shared<ItemImpl>(hWnd,pnmtv->itemNew.hItem))
                            );
                            if( ! allowCollapse ) return TRUE;
                        }
                        } break;
                } break;
            case TVN_ITEMEXPANDED: {
                auto pnmtv = reinterpret_cast<LPNMTREEVIEW>(lParam);
                switch(pnmtv->action) {
                    case TVE_EXPAND:
                        if( findExecItem(cbOnExpand,this,pnmtv->itemNew.hItem) ) return 0;
                        break;
                    case TVE_COLLAPSE:
                        if( findExecItem(cbOnCollapse,this,pnmtv->itemNew.hItem) ) return 0;
                        break;
                }
                } break;
        }
	}

    return NativeControlImpl::processNotification(message,notification,wParam,lParam);
}

void TreeViewImpl::clearItems()
{
    if( ! TreeView_DeleteAllItems(hWnd) ) {
        // TODO log error
    }
}

TreeView::Item TreeViewImpl::getParent(TreeView::Item const & item) const
{
    HTREEITEM hParent = item.itemImpl->parent();
    if( hParent == 0 ) return root();
    
    return TreeView::Item(std::make_shared<ItemImpl>(hWnd,hParent));
}


/* ItemImpl */

ItemImpl::ItemImpl(HWND hTreeView, HTREEITEM hItem):
    hTreeView(hTreeView),
    hItem(hItem),
    hTail(0)
{}

bool ItemImpl::operator==(ItemImpl const & i) const
{
    return hTreeView == i.hTreeView && hItem == i.hItem;
}

HTREEITEM ItemImpl::firstChild() const
{
    return TreeView_GetChild(hTreeView, hItem);
}

HTREEITEM ItemImpl::nextSibling() const
{
    return TreeView_GetNextSibling(hTreeView, hItem);
}

HTREEITEM ItemImpl::parent() const
{
    return TreeView_GetParent(hTreeView, hItem);
}

ItemProperties ItemImpl::getProperties() const
{
    ItemProperties result;
    wchar_t buffer[MAX_TREEVIEW_ITEM_CHARS];
    TVITEMW item;
    item.mask = TVIF_HANDLE | TVIF_TEXT | TVIF_IMAGE;
    item.hItem = hItem;
    item.pszText = &buffer[0];
    item.cchTextMax = sizeof(buffer) - 1;
    if( SendMessageW(hTreeView,TVM_GETITEMW,0,reinterpret_cast<LPARAM>(&item)) ) {
        result.text.wstr = std::wstring(buffer);
        result.imageIndex = item.iImage;
    } else {
        // TODO log error
    }
    return result;
}

HTREEITEM ItemImpl::addChild(ItemProperties const & properties)
{
    // compute tail if it was not computed yet
    if( hTail == 0 ) {
        hTail = firstChild();
        if( hTail != 0 ) {
            for( HTREEITEM hItem = hTail;
                    hItem != 0;
                    hItem = TreeView_GetNextSibling(hTreeView, hItem) ) {
                hTail = hItem;
            }
        }
    }
    
    TVINSERTSTRUCTW tvins;
    tvins.hParent = hItem;
    tvins.hInsertAfter = hTail != 0 ? hTail : TVI_FIRST;
    tvins.item.mask = TVIF_TEXT;
    tvins.item.pszText = const_cast<wchar_t *>(properties.text.wstr.c_str());
    if( properties.imageIndex != ImageList::noIndex ) {
        tvins.item.mask |= TVIF_IMAGE | TVIF_SELECTEDIMAGE;
        tvins.item.iImage = properties.imageIndex;
        tvins.item.iSelectedImage = properties.imageIndex;
    }
    
    HTREEITEM hItem = (HTREEITEM) SendMessageW(hTreeView,TVM_INSERTITEMW,0,(LPARAM)&tvins);
    if( hItem ) {
        hTail = hItem;
    } else {
        // TODO log error
    }
    
    return hItem;
}

/* Item */

TreeView::Item::Item(std::shared_ptr<ItemImpl> itemImpl):
    itemImpl(itemImpl)
{
}

TreeView::Item::Item(TreeView::Item const & item):
    itemImpl(item.itemImpl)
{
}

TreeView::Item & TreeView::Item::operator=(TreeView::Item const & item)
{
    itemImpl = item.itemImpl;
    return *this;
}

TreeView::Item::~Item()
{
}

ItemProperties TreeView::Item::getProperties() const
{
    return itemImpl->getProperties();
}

void TreeView::Item::setText(die::NativeString const & text)
{
    TVITEMW item;
    item.mask = TVIF_HANDLE | TVIF_TEXT;
    item.hItem = itemImpl->hItem;
    item.pszText = const_cast<wchar_t *>(text.wstr.c_str());
    if( ! SendMessageW(itemImpl->hTreeView,TVM_SETITEMW,0,reinterpret_cast<LPARAM>(&item)) ) {
        // TODO log error        
    }
}

void TreeView::Item::setImageIndex(ImageList::Index imageIndex)
{
    TVITEM item;
    item.mask = TVIF_HANDLE | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
    item.hItem = itemImpl->hItem;
    item.iImage = imageIndex;
    item.iSelectedImage = imageIndex;
    if( ! TreeView_SetItem(itemImpl->hTreeView,&item) ) {
        // TODO log error
    }
}

TreeView::Iterator TreeView::Item::addChild(ItemProperties const & properties)
{
    HTREEITEM hItem = itemImpl->addChild(properties);
    return Iterator(new IteratorImpl(itemImpl->hTreeView,hItem));
}

void TreeView::Item::eraseChild(TreeView::Iterator const & it)
{
    // TODO assert treeImpl->parent.hTreeView = it->...treeView
    HTREEITEM hItem = it.iteratorImpl->itemImpl.hItem;
    HTREEITEM newTail =
        itemImpl->hTail == hItem ?
        TreeView_GetPrevSibling(itemImpl->hTreeView,itemImpl->hTail) :
        itemImpl->hTail;
    if( TreeView_DeleteItem(it.iteratorImpl->itemImpl.hTreeView,hItem) ) {
        itemImpl->hTail = newTail;
    } else {
        // TODO log error
    }
}

TreeView::Iterator TreeView::Item::begin()
{
    HTREEITEM hChild = itemImpl->firstChild();
    return Iterator(new IteratorImpl(itemImpl->hTreeView,hChild));
}

TreeView::Iterator TreeView::Item::end()
{
    return Iterator(new IteratorImpl(itemImpl->hTreeView,0));
}

bool TreeView::Item::empty() const
{
    HTREEITEM hChild = itemImpl->firstChild();
    return hChild == 0;
}

bool TreeView::Item::operator==(Item const & item) const
{
    return *itemImpl == *item.itemImpl;
}

}
