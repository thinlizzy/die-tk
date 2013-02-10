#ifndef TREEVIEW_H_hfdh43529jfgjk3432gf0sdj
#define TREEVIEW_H_hfdh43529jfgjk3432gf0sdj

#include "Control.h"
#include <string>
#include <iterator>
#include <memory>
#include "../components/ImageList.h"
#include "../util/ClonePtr.h"

namespace tk {

class ItemImpl;
class IteratorImpl;

class TreeView: public virtual Control {
public:
    class Item;
    class Iterator;
    
    virtual Item root() = 0;
    virtual size_t total() const = 0;
    virtual void setImageList(std::shared_ptr<ImageList> imageList) = 0;
    virtual std::shared_ptr<ImageList> getImageList() = 0;
    virtual Iterator selected() const = 0;

    typedef std::function<bool(Item)> AllowItemChange;
    typedef std::function<bool(Item,Item)> AllowChangeFromTo;
    typedef std::function<void(Item)> HandleItemOperation;

    using Control::onMouse;
    using Control::onClick;
    
    virtual void beforeChange(AllowChangeFromTo callback) = 0;
    virtual void onChange(HandleItemOperation callback) = 0;
    virtual void beforeExpand(AllowItemChange callback) = 0;
    virtual void onExpand(HandleItemOperation callback) = 0;
    virtual void beforeCollapse(AllowItemChange callback) = 0;
    virtual void onCollapse(HandleItemOperation callback) = 0;
    
    struct ItemProperties {
    public:
        std::string text;
        ImageList::Index imageIndex;
        ItemProperties(): imageIndex(ImageList::noIndex) {}
        ItemProperties & setImageIndex(ImageList::Index imageIndex) { this->imageIndex = imageIndex; return *this; }        
        ItemProperties & setText(std::string const & text) { this->text = text; return *this; }
    };

    class Item {
        friend class IteratorImpl;
    public:
        Item(Item const & Item);
        ~Item();
        Item & operator=(Item const & item);

        ItemProperties getProperties() const;
        void setText(std::string const & text);
        void setImageIndex(ImageList::Index imageIndex);

        Iterator addChild(ItemProperties const & properties);
        void eraseChild(Iterator const & it);   // invalidates it

        Iterator begin();
        Iterator end();

        bool empty() const;
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
