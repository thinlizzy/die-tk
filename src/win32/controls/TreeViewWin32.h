#ifndef TREEVIEWWIN32_H_jjb0387gk43n34hdfhgsdsdf
#define TREEVIEWWIN32_H_jjb0387gk43n34hdfhgsdsdf

#include "NativeControlWin32.h"
#include "../../controls/TreeView.h"
#include <commctrl.h>
#include "../CommonControlInitializer.h"

#pragma warning( push )
#pragma warning( disable : 4250 )

namespace tk {

class TreeViewImpl: public NativeControlImpl, public TreeView, private CommonControlInitializer<ICC_TREEVIEW_CLASSES> {
    TreeView::Item rootItem;
public:
	TreeViewImpl(HWND parent_hWnd, ControlParams const & params);

    virtual TreeView::Item & root();
    virtual size_t total() const;
};

class ItemImpl {
public:
    HWND hTreeView;
    HTREEITEM hParent;
    HTREEITEM hItem;
    HTREEITEM hTail;

    ItemImpl(HWND hTreeView, HTREEITEM hItem, HTREEITEM hParent);
    bool operator==(ItemImpl const & ih) const;
    HTREEITEM firstChild() const;
    HTREEITEM nextSibling() const;
};

class IteratorImpl {
public:
    TreeView::Item item;
    IteratorImpl() = default;
    IteratorImpl(HWND hTreeView, HTREEITEM hItem, HTREEITEM hParent): item(new ItemImpl(hTreeView,hItem,hParent)) {}
    ItemImpl & itemImpl() { return *item.itemImpl; }
    ItemImpl const & itemImpl() const { return *item.itemImpl; }
};

}

#pragma warning( pop )

#endif
