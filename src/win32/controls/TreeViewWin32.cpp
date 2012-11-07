#include "TreeViewWin32.h"

namespace tk {

/* TreeViewImpl */

TreeViewImpl::TreeViewImpl(HWND parent_hWnd, ControlParams const & params):
	NativeControlImpl(parent_hWnd,params,WC_TREEVIEW,TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS),
	itemTree(new TreeImpl(hWnd,TVI_ROOT))
{
}

TreeView::Tree & TreeViewImpl::items()
{
    return itemTree;
}

size_t TreeViewImpl::total() const
{
    return TreeView_GetCount(hWnd);
}

/* ItemHandle */

ItemHandle::ItemHandle():
    hTreeView(0),hItem(0)
{}

ItemHandle::ItemHandle(HWND hTreeView, HTREEITEM hItem):
    hTreeView(hTreeView),hItem(hItem)
{}

bool ItemHandle::isSet() const
{
    return hTreeView != 0;
}

bool ItemHandle::operator==(ItemHandle const & ih) const
{
    return hTreeView == ih.hTreeView && hItem == ih.hItem;
}

void ItemHandle::next()
{
    hItem = TreeView_GetNextSibling(hTreeView, hItem);
}

HTREEITEM ItemHandle::firstChild() const
{
    return TreeView_GetChild(hTreeView, hItem);
}

/* ItemImpl */

ItemImpl::ItemImpl(HWND hTreeView, HTREEITEM hItem):
    handle(hTreeView,hItem)
{
}

std::string ItemImpl::getText() const
{
    if( handle.isSet() && text.empty() ) {
        char buffer[261];
        TVITEM item;
        item.mask = TVIF_HANDLE | TVIF_TEXT;
        item.hItem = handle.hItem;
        item.pszText = &buffer[0];
        item.cchTextMax = sizeof(buffer) - 1;
        if( TreeView_GetItem(handle.hTreeView,&item) ) {
            text = buffer;
        } else {
            // TODO log error
        }
    }

    return text;
}

void ItemImpl::setText(std::string const & text)
{
    if( handle.isSet() ) {
        TVITEM item;
        item.mask = TVIF_HANDLE | TVIF_TEXT;
        item.hItem = handle.hItem;
        item.pszText = const_cast<char *>(text.c_str());
        if( TreeView_SetItem(handle.hTreeView,&item) ) {
            this->text = text;
        } else {
            // TODO log error
        }
    } else {
        this->text = text;
    }
}

/* Item */

TreeView::Item::Item():
    itemImpl(new ItemImpl)
{
}

TreeView::Item::Item(ItemImpl * itemImpl):
    itemImpl(itemImpl)
{
    // TODO assert itemImpl != 0
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

std::string TreeView::Item::getText() const
{
    return itemImpl->getText();
}

TreeView::Item & TreeView::Item::setText(std::string const & text)
{
    itemImpl->setText(text);
    return *this;
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

TreeView::Item & TreeView::Iterator::operator*()
{
    return iteratorImpl->item;
}

TreeView::Item * TreeView::Iterator::operator->()
{
    return &operator*();
}

TreeView::Iterator & TreeView::Iterator::operator++()
{
    iteratorImpl->itemHandle().next();
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
    return iteratorImpl->itemHandle() == it.iteratorImpl->itemHandle();
}

TreeView::Tree TreeView::Iterator::children()
{
    return Tree(new TreeImpl(iteratorImpl->itemHandle()));
}

/* Tree */

TreeView::Tree::Tree(TreeImpl * treeImpl):
    treeImpl(treeImpl)
{
}

TreeView::Tree::Tree(TreeView::Tree const & tree):
    treeImpl(tree.treeImpl)
{
}

TreeView::Tree::~Tree()
{
}

TreeView::Iterator TreeView::Tree::add(TreeView::Item const & item)
{
    // compute tail if it was not computed yet
    if( treeImpl->hTail == 0 ) {
        treeImpl->hTail = treeImpl->parent.firstChild();
        if( treeImpl->hTail != 0 ) {
            for( HTREEITEM hItem = treeImpl->hTail;
                    hItem != 0;
                    hItem = TreeView_GetNextSibling(treeImpl->parent.hTreeView, hItem) ) {
                treeImpl->hTail = hItem;
            }
        }
    }

    TVINSERTSTRUCT tvins;
    tvins.hParent = treeImpl->parent.hItem;
    tvins.hInsertAfter = treeImpl->hTail != 0 ? treeImpl->hTail : TVI_FIRST;
    tvins.item.mask = TVIF_TEXT;
    tvins.item.pszText = const_cast<char *>(item.getText().c_str());
    // TODO TVIF_IMAGE | TVIF_SELECTEDIMAGE
    HTREEITEM hItem = TreeView_InsertItem(treeImpl->parent.hTreeView,&tvins);
    if( hItem ) {
        treeImpl->hTail = hItem;
    } else {
        // TODO log error
    }

    return Iterator(new IteratorImpl(treeImpl->parent.hTreeView,hItem));
}

void TreeView::Tree::erase(TreeView::Iterator it)
{
    // TODO assert treeImpl->parent.hTreeView = it->...treeView
    HTREEITEM hItem = it.iteratorImpl->itemHandle().hItem;
    HTREEITEM newTail =
        treeImpl->hTail == hItem ?
        TreeView_GetPrevSibling(treeImpl->parent.hTreeView,treeImpl->hTail) :
        treeImpl->hTail;
    if( TreeView_DeleteItem(it.iteratorImpl->itemHandle().hTreeView,hItem) ) {
        treeImpl->hTail = newTail;
    } else {
        // TODO log error
    }
}

TreeView::Iterator TreeView::Tree::begin()
{
    HTREEITEM hChild = treeImpl->parent.firstChild();
    return Iterator(new IteratorImpl(treeImpl->parent.hTreeView,hChild));
}

TreeView::Iterator TreeView::Tree::end()
{
    return Iterator(new IteratorImpl(treeImpl->parent.hTreeView,0));
}

bool TreeView::Tree::empty() const
{
    HTREEITEM hChild = treeImpl->parent.firstChild();
    return hChild == 0;
}

}
