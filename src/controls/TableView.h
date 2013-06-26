#ifndef TABLE_VIEW_H_jff43ufdgnvcbcvjvn23
#define TABLE_VIEW_H_jff43ufdgnvcbcvjvn23

#include <initializer_list>

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

    int rows() const;
    int columns() const;
    
    void addColumn(ColumnProperties columnProp);
    void deleteColumn(int c);
    void setColumn(int c, ColumnProperties columnProp);
    void setColumns(std::initializer_list<ColumnProperties> columns);
    ColumnProperties column(int c) const;

    void addRow(std::initializer_list<ItemProperties> items);
    
    void setRows(int r);
    void deleteRow(int r);
    
    void setItem(int c, int r, ItemProperties itemProp);
    ItemProperties item(int c, int r) const;
    
    void setImageList(ImageList & imageList);
    void removeImageList();
    optional<ImageList> getImageList();

    // TODO define callbacks
    using Control::onMouse;
};

}


#endif
