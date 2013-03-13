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
    virtual ~TreeViewImpl();

    virtual TreeView::Item root() const;
    virtual size_t total() const;
    virtual void setImageList(std::shared_ptr<ImageList> imageList);
    virtual std::shared_ptr<ImageList> getImageList();
    virtual TreeView::Iterator selected() const;
    
    virtual Item getParent(Item const & item) const;
    
    virtual void clear();

    virtual void beforeChange(TreeView::AllowChangeFromTo callback);
    virtual void onChange(TreeView::HandleItemOperation callback);
    virtual void beforeExpand(TreeView::AllowItemChange callback);
    virtual void onExpand(TreeView::HandleItemOperation callback);
    virtual void beforeCollapse(TreeView::AllowItemChange callback);
    virtual void onCollapse(TreeView::HandleItemOperation callback);
    
    virtual optional<LRESULT> processNotification(UINT message, UINT notification, WPARAM wParam, LPARAM lParam);    
};

class ItemImpl {
public:
    HWND hTreeView;
    HTREEITEM hItem;
    HTREEITEM hTail;

    ItemImpl(HWND hTreeView, HTREEITEM hItem);
    bool operator==(ItemImpl const & ih) const;
    HTREEITEM firstChild() const;
    HTREEITEM nextSibling() const;
    HTREEITEM parent() const;
};

class IteratorImpl {
public:
    ItemImpl itemImpl;    
    IteratorImpl() = default;
    IteratorImpl(HWND hTreeView, HTREEITEM hItem): itemImpl(hTreeView,hItem) {}
};

}

#pragma warning( pop )

#endif
