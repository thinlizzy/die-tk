#ifndef TREEVIEW_H_hfdh43529jfgjk3432gf0sdj
#define TREEVIEW_H_hfdh43529jfgjk3432gf0sdj

#include "Control.h"
#include <string>
#include <iterator>
#include "../util/ClonePtr.h"

// TODO design imagelist and handlers
// TODO add extra image argument to add functions
// TODO define callbacks

namespace tk {

class ItemImpl;
class TreeImpl;
class IteratorImpl;

class TreeView: public virtual Control {
public:
    class Tree;
    class Iterator;

    virtual Tree & items() = 0;
    virtual size_t total() const = 0;

    class Item {
        friend class IteratorImpl;
    public:
        Item();
        Item(Item const & item);
        Item & operator=(Item const & item);
        ~Item();

        std::string getText() const;
        Item & setText(std::string const & text);
        // TODO set image
        // TODO get image
    private:
        ClonePtr<ItemImpl> itemImpl;
        explicit Item(ItemImpl * itemImpl);
    };

    class Tree {
    public:
        Tree(Tree const & tree);
        ~Tree();
        Iterator begin();
        Iterator end();
        Iterator add(Item const & item);
        void erase(Iterator it);
        bool empty() const;
    private:
        ClonePtr<TreeImpl> treeImpl;
    public:
        explicit Tree(TreeImpl * treeImpl);
    };

    class Iterator: public std::iterator<std::forward_iterator_tag,Item> {
        friend class Tree;
    public:
        Iterator() = default;
        Iterator(Iterator const & it);
        Iterator & operator=(Iterator const & it);
        ~Iterator();

        Item & operator*();
        Item * operator->();
        Iterator & operator++();
        Iterator operator++(int);
        bool operator==(Iterator const & it) const;

        Tree children();
    private:
        ClonePtr<IteratorImpl> iteratorImpl;
    public:
        explicit Iterator(IteratorImpl * iteratorImpl);
    };
};

}

#endif
