#ifndef TABLEVIEW_WIN32_H_j4320fglvcmew8113g
#define TABLEVIEW_WIN32_H_j4320fglvcmew8113g

#include "NativeControlWin32.h"
#include "../CommonControlInitializer.h"
#include "../../controls/base/ColumnProperties.h"
#include "../../controls/base/ItemProperties.h"
#include <commctrl.h>

namespace tk {

class TableViewImpl: public NativeControlImpl, private CommonControlInitializer<ICC_LISTVIEW_CLASSES> {
    int colCount;
    int rowCount;
    std::shared_ptr<ImageListImpl> imageListImpl;
public:
	TableViewImpl(Window & parent, ControlParams const & params);
    ~TableViewImpl();

    int rows() const;
    int columns() const;
    
    void addColumn(ColumnProperties columnProp);
    void setColumn(int c, ColumnProperties columnProp);
    void deleteColumn(int c);
    ColumnProperties column(int c) const;
    
    void setRows(int r);
    void deleteRow(int r);
    
    void setItem(int c, int r, ItemProperties itemProp);
    ItemProperties item(int c, int r) const;
    
    int newRow(ItemProperties itemProp);
    
    void setImageList(ImageList & imageList);
    void removeImageList();
    optional<ImageList> getImageList();
private:
    bool addItem(int r, ItemProperties itemProp);
};

}

#endif
