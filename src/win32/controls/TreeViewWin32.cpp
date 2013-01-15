#include "TreeViewWin32.h"
#include "../components/ImageListWin32.h"

namespace tk {

/* TreeViewImpl */

TreeViewImpl::TreeViewImpl(HWND parent_hWnd, ControlParams const & params):
	NativeControlImpl(parent_hWnd,params,WC_TREEVIEW,TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS),
	rootItemImpl(std::make_shared<ItemImpl>(hWnd,TVI_ROOT,HTREEITEM(0)))
{
}

TreeView::Item TreeViewImpl::root()
{
    return TreeView::Item(rootItemImpl);
}

size_t TreeViewImpl::total() const
{
    return TreeView_GetCount(hWnd);
}

void TreeViewImpl::setImageList(std::shared_ptr<ImageList> imageList)
{
    // set the new image list
    auto & imageListImpl = dynamic_cast<ImageListImpl &>(*imageList);
    auto hOld = TreeView_SetImageList(hWnd, imageListImpl.getHiml(), TVSIL_NORMAL);
    // TODO if hOld != null , assert hOld equals handle of old one
    // replace the old one
    this->imageList = imageList;
}

std::shared_ptr<ImageList> TreeViewImpl::getImageList()
{
    return imageList;
}

/* ItemImpl */

ItemImpl::ItemImpl(HWND hTreeView, HTREEITEM hItem, HTREEITEM hParent):
    hTreeView(hTreeView),
    hParent(hParent),
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

TreeView::ItemProperties TreeView::Item::getProperties() const
{
    ItemProperties result;
    char buffer[261];
    TVITEM item;
    item.mask = TVIF_HANDLE | TVIF_TEXT;
    item.hItem = itemImpl->hItem;
    item.pszText = &buffer[0];
    item.cchTextMax = sizeof(buffer) - 1;
    if( TreeView_GetItem(itemImpl->hTreeView,&item) ) {
        result.text = buffer;
        // TODO get image index too
    } else {
        // TODO log error
    }
    return result;
}

void TreeView::Item::setText(std::string const & text)
{
    TVITEM item;
    item.mask = TVIF_HANDLE | TVIF_TEXT;
    item.hItem = itemImpl->hItem;
    item.pszText = const_cast<char *>(text.c_str());
    if( ! TreeView_SetItem(itemImpl->hTreeView,&item) ) {
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

TreeView::Iterator TreeView::Item::addChild(TreeView::ItemProperties const & properties)
{
    // compute tail if it was not computed yet
    if( itemImpl->hTail == 0 ) {
        itemImpl->hTail = itemImpl->firstChild();
        if( itemImpl->hTail != 0 ) {
            for( HTREEITEM hItem = itemImpl->hTail;
                    hItem != 0;
                    hItem = TreeView_GetNextSibling(itemImpl->hTreeView, hItem) ) {
                itemImpl->hTail = hItem;
            }
        }
    }

    TVINSERTSTRUCT tvins;
    tvins.hParent = itemImpl->hItem;
    tvins.hInsertAfter = itemImpl->hTail != 0 ? itemImpl->hTail : TVI_FIRST;
    tvins.item.mask = TVIF_TEXT;
    tvins.item.pszText = const_cast<char *>(properties.text.c_str());
    if( properties.imageIndex != ImageList::noIndex ) {
        tvins.item.mask |= TVIF_IMAGE | TVIF_SELECTEDIMAGE;
        tvins.item.iImage = properties.imageIndex;
        tvins.item.iSelectedImage = properties.imageIndex;
    }
    
    HTREEITEM hItem = TreeView_InsertItem(itemImpl->hTreeView,&tvins);
    if( hItem ) {
        itemImpl->hTail = hItem;
    } else {
        // TODO log error
    }

    return Iterator(new IteratorImpl(itemImpl->hTreeView,hItem,tvins.hParent));
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
    return Iterator(new IteratorImpl(itemImpl->hTreeView,hChild,itemImpl->hItem));
}

TreeView::Iterator TreeView::Item::end()
{
    return Iterator(new IteratorImpl(itemImpl->hTreeView,0,itemImpl->hItem));
}

bool TreeView::Item::empty() const
{
    HTREEITEM hChild = itemImpl->firstChild();
    return hChild == 0;
}

/* Iterator */

TreeView::Iterator::Iterator(IteratorImpl * iteratorImpl):
    iteratorImpl(iteratorImpl)
{
}

TreeView::Iterator::Iterator(TreeView::Iterator const & it):
    iteratorImpl(it.iteratorImpl)
{
}

TreeView::Iterator & TreeView::Iterator::operator=(TreeView::Iterator const & it)
{
    iteratorImpl = it.iteratorImpl;
    return *this;
}

TreeView::Iterator::~Iterator()
{
}

TreeView::Item TreeView::Iterator::operator*()
{
    return TreeView::Item(std::make_shared<ItemImpl>(iteratorImpl->itemImpl));
}

TreeView::Iterator & TreeView::Iterator::operator++()
{
    iteratorImpl->itemImpl.hItem = iteratorImpl->itemImpl.nextSibling();
    return *this;
}

TreeView::Iterator TreeView::Iterator::operator++(int)
{
    auto result = *this;
    operator++();
    return result;
}

bool TreeView::Iterator::operator==(TreeView::Iterator const & it) const
{
    if( ! iteratorImpl ) return ! it.iteratorImpl;
    if( ! it.iteratorImpl ) return false;

    return iteratorImpl->itemImpl == it.iteratorImpl->itemImpl;
}

}
