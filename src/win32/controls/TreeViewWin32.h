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
    std::shared_ptr<ItemImpl> rootItemImpl;
    std::shared_ptr<ImageList> imageList;
public:
	TreeViewImpl(HWND parent_hWnd, ControlParams const & params);

    virtual TreeView::Item root();
    virtual size_t total() const;
    virtual void setImageList(std::shared_ptr<ImageList> imageList);
    virtual std::shared_ptr<ImageList> getImageList();
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
    ItemImpl itemImpl;    
    IteratorImpl() = default;
    IteratorImpl(HWND hTreeView, HTREEITEM hItem, HTREEITEM hParent): itemImpl(hTreeView,hItem,hParent) {}
};

}

#pragma warning( pop )

#endif
