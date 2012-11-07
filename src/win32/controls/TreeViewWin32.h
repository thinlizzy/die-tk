#ifndef TREEVIEWWIN32_H_jjb0387gk43n34hdfhgsdsdf
#define TREEVIEWWIN32_H_jjb0387gk43n34hdfhgsdsdf

#include "NativeControlWin32.h"
#include "../../controls/TreeView.h"
#include <commctrl.h>

#pragma warning( push )
#pragma warning( disable : 4250 )

namespace tk {

class TreeViewImpl: public NativeControlImpl, public TreeView {
    Tree itemTree;
public:
	TreeViewImpl(HWND parent_hWnd, ControlParams const & params);

    virtual Tree & items();
    virtual size_t total() const;
};

class ItemHandle {
public:
    HWND hTreeView;
    HTREEITEM hItem;

    ItemHandle();
    ItemHandle(HWND hTreeView, HTREEITEM hItem);
    bool isSet() const;
    bool operator==(ItemHandle const & ih) const;
    void next();
    HTREEITEM firstChild() const;
};

class ItemImpl {
    mutable std::string text;
    // TODO add image index here
public:
    ItemHandle handle;

    ItemImpl() = default;
    ItemImpl(HWND hTreeView, HTREEITEM hItem);
    std::string getText() const;
    void setText(std::string const & text);
};

class IteratorImpl {
public:
    TreeView::Item item;
    IteratorImpl() = default;
    IteratorImpl(HWND hTreeView, HTREEITEM hItem): item(new ItemImpl(hTreeView,hItem)) {}
    ItemHandle & itemHandle() { return item.itemImpl->handle; }
    ItemHandle const & itemHandle() const { return item.itemImpl->handle; }
};

class TreeImpl {
public:
    ItemHandle parent;
    HTREEITEM hTail;
    explicit TreeImpl(ItemHandle parent): parent(parent),hTail(0) {}
    TreeImpl(HWND hTreeView, HTREEITEM hItem): parent(hTreeView,hItem),hTail(0) {}
};

}

#pragma warning( pop )

#endif
