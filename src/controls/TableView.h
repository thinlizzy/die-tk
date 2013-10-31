#ifndef TABLE_VIEW_H_jff43ufdgnvcbcvjvn23
#define TABLE_VIEW_H_jff43ufdgnvcbcvjvn23

#include <initializer_list>
#include <ostream>

#include "../Control.h"
#include "../components/ImageList.h"
#include "../util/optional.h"
#include "base/ItemProperties.h"
#include "base/ColumnProperties.h"

namespace tk {
    
class TableView: public Control {
public:
    TableView() = default;
	TableView(Window & parent, ControlParams const & params);
    TableView clone() const;
    
    struct ItemPos {
        int c;
        int r;
    };

    int rows() const;
    int columns() const;
    
    void addColumn(ColumnProperties columnProp);
    void deleteColumn(int c);
    void setColumn(int c, ColumnProperties columnProp);
    void setColumns(std::initializer_list<ColumnProperties> columns);
    ColumnProperties column(int c) const;

    void addRow(std::initializer_list<ItemProperties> items);
    void addRow(std::initializer_list<die::NativeString> items);
    
    void setRows(int r);
    void deleteRow(int r);
    
    void setItem(int c, int r, ItemProperties itemProp);
    void setItem(ItemPos pos, ItemProperties itemProp);
    ItemProperties item(int c, int r) const;
    ItemProperties item(ItemPos pos) const;
    
    ItemPos getItemPos(Point point) const;
    
    void setImageList(ImageList & imageList);
    void removeImageList();
    optional<ImageList> getImageList();
    
    void setGridLines(bool drawGrid);
    void setRowSelect(bool rowSelect);

    typedef std::function<bool(ItemPos, Canvas &, Rect)> DrawItem;
    typedef std::function<void(ItemPos)> ItemEvent;

    DrawItem onDrawItem(DrawItem callback);
    ItemEvent onClickItem(ItemEvent callback);
    // TODO define other callbacks
private:
    template<typename T>
    void doAddRow(std::initializer_list<T> items);

    using Control::getText;
    using Control::setText;
};

inline std::ostream & operator<<(std::ostream & os, TableView::ItemPos const & itemPos)
{
    os << '(' << itemPos.c << ',' << itemPos.r << ')';
    return os;
}

}


#endif
