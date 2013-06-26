#ifndef TREEVIEW_H_hfdh43529jfgjk3432gf0sdj
#define TREEVIEW_H_hfdh43529jfgjk3432gf0sdj

#include <string>
#include <iterator>
#include <memory>
#include <functional>
#include "../Control.h"
#include "../components/ImageList.h"
#include "../util/ClonePtr.h"
#include "../util/optional.h"
#include "base/ItemProperties.h"

namespace tk {

class ItemImpl;
class IteratorImpl;

class TreeView: public Control {
public:
    class Item;
    class Iterator;
    
    TreeView() = default;
	TreeView(Window & parent, ControlParams const & params);
    
    Item root() const;
    size_t total() const;
    Iterator selected() const;
    
    Item getParent(Item const & item) const;

    void clearItems();

    void setImageList(ImageList & imageList);
    void removeImageList();
    optional<ImageList> getImageList();
    
    typedef std::function<bool(Item)> AllowItemChange;
    typedef std::function<bool(Item,Item)> AllowChangeFromTo;
    typedef std::function<void(Item)> HandleItemOperation;

    using Control::onMouse;
    AllowChangeFromTo beforeChange(AllowChangeFromTo callback);
    HandleItemOperation onChange(HandleItemOperation callback);
    AllowItemChange beforeExpand(AllowItemChange callback);
    HandleItemOperation onExpand(HandleItemOperation callback);
    AllowItemChange beforeCollapse(AllowItemChange callback);
    HandleItemOperation onCollapse(HandleItemOperation callback);
    
    class Item {
        friend class IteratorImpl;
        friend class TreeViewImpl;
    public:
        Item(Item const & Item);
        ~Item();
        Item & operator=(Item const & item);
        bool operator==(Item const & item) const;
        
        ItemProperties getProperties() const;
        void setText(die::NativeString const & text);
        void setImageIndex(ImageList::Index imageIndex);

        Iterator addChild(ItemProperties const & properties);
        void eraseChild(Iterator const & it);    // invalidates it
        
        Iterator begin();      // first child
        Iterator end();
        
        bool empty() const;    // empty if item has no children
    private:
        std::shared_ptr<ItemImpl> itemImpl;
        Item() = default;
    public:
        explicit Item(std::shared_ptr<ItemImpl> itemImpl);
    };

    class Iterator: public std::iterator<std::forward_iterator_tag,Item> {
        friend class Item;
    public:
        Iterator() = default;
        Iterator(Iterator const & it);
        Iterator & operator=(Iterator const & it);
        ~Iterator();

        Item operator*();
        Iterator & operator++();
        Iterator operator++(int);
        bool operator==(Iterator const & it) const;
        explicit operator bool() const;
    private:
        ClonePtr<IteratorImpl> iteratorImpl;
    public:
        explicit Iterator(IteratorImpl * iteratorImpl);
    };
};

}

#endif
