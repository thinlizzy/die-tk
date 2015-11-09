#ifndef TREEVIEWWIN32_H_jjb0387gk43n34hdfhgsdsdf
#define TREEVIEWWIN32_H_jjb0387gk43n34hdfhgsdsdf

#include "../../../win32/die-tk-win32.h"
#include <commctrl.h>
#include "../../src/controls/TreeView.h"
#include "../CommonControlInitializer.h"

namespace tk {

class TreeViewImpl: public NativeControlImpl, private CommonControlInitializer<ICC_TREEVIEW_CLASSES> {
    std::shared_ptr<ItemImpl> rootItemImpl;
    std::shared_ptr<ImageListImpl> imageListImpl;
public:
	TreeViewImpl(HWND parentHwnd, ControlParams const & params);
    ~TreeViewImpl();

    virtual TreeViewImpl * clone() const;
    
    TreeView::Item root() const;
    size_t total() const;
    
    void setImageList(ImageList & imageList);
    void removeImageList();
    optional<ImageList> getImageList();
    
    TreeView::Iterator selected() const;
    
    TreeView::Item getParent(TreeView::Item const & item) const;
    
    void clearItems();

    TreeView::AllowChangeFromTo beforeChange(TreeView::AllowChangeFromTo callback);
    TreeView::HandleItemOperation onChange(TreeView::HandleItemOperation callback);
    TreeView::AllowItemChange beforeExpand(TreeView::AllowItemChange callback);
    TreeView::HandleItemOperation onExpand(TreeView::HandleItemOperation callback);
    TreeView::AllowItemChange beforeCollapse(TreeView::AllowItemChange callback);
    TreeView::HandleItemOperation onCollapse(TreeView::HandleItemOperation callback);
private:
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
    
    ItemProperties getProperties() const;
    HTREEITEM addChild(ItemProperties const & properties);
};

class IteratorImpl {
public:
    ItemImpl itemImpl;    
    IteratorImpl() = default;
    IteratorImpl(HWND hTreeView, HTREEITEM hItem): itemImpl(hTreeView,hItem) {}
};

}

#endif
