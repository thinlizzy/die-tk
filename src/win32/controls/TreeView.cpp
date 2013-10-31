#include "../../controls/TreeView.h"
#include "TreeViewWin32.h"

#define CONTROL_IMPL TreeViewImpl
#include "ControlMacros.h"

namespace tk {

CONSTRUCTOR_IMPL(TreeView);
CLONE_IMPL(TreeView);

TreeView::Item TreeView::root() const
{
    return IMPL.root();
}

size_t TreeView::total() const
{
    return IMPL.total();
}

TreeView::Iterator TreeView::selected() const
{
    return IMPL.selected();
}

TreeView::Item TreeView::getParent(TreeView::Item const & item) const
{
    return IMPL.getParent(item);
}

void TreeView::clearItems()
{
    IMPL.clearItems();
}

void TreeView::setImageList(ImageList & imageList)
{
    IMPL.setImageList(imageList);
}

void TreeView::removeImageList()
{
    IMPL.removeImageList();
}

optional<ImageList> TreeView::getImageList()
{
    return IMPL.getImageList();
}

TreeView::AllowChangeFromTo TreeView::beforeChange(TreeView::AllowChangeFromTo callback)
{
    return IMPL.beforeChange(callback);
}

TreeView::HandleItemOperation TreeView::onChange(TreeView::HandleItemOperation callback)
{
    return IMPL.onChange(callback);
}

TreeView::AllowItemChange TreeView::beforeExpand(TreeView::AllowItemChange callback)
{
    return IMPL.beforeExpand(callback);
}

TreeView::HandleItemOperation TreeView::onExpand(TreeView::HandleItemOperation callback)
{
    return IMPL.onExpand(callback);
}

TreeView::AllowItemChange TreeView::beforeCollapse(TreeView::AllowItemChange callback)
{
    return IMPL.beforeCollapse(callback);
}

TreeView::HandleItemOperation TreeView::onCollapse(TreeView::HandleItemOperation callback)
{
    return IMPL.onCollapse(callback);
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

TreeView::Iterator::operator bool() const
{
    if( ! iteratorImpl ) return false;
    
    return iteratorImpl->itemImpl.hItem != 0;
}

}
