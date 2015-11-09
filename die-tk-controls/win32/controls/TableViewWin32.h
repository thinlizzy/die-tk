#ifndef TABLEVIEW_WIN32_H_j4320fglvcmew8113g
#define TABLEVIEW_WIN32_H_j4320fglvcmew8113g

#include "../../../win32/die-tk-win32.h"
#include <commctrl.h>
#include "../CommonControlInitializer.h"
#include "../../src/controls/base/ColumnProperties.h"
#include "../../src/controls/base/ItemProperties.h"
#include "../../src/controls/TableView.h"

namespace tk {

class TableViewImpl: public NativeControlImpl, private CommonControlInitializer<ICC_LISTVIEW_CLASSES> {
    int colCount;
    int rowCount;
    std::shared_ptr<ImageListImpl> imageListImpl;
public:
	TableViewImpl(HWND parentHwnd, ControlParams const & params);
    ~TableViewImpl();

    virtual TableViewImpl * clone() const;
    
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
    
    TableView::ItemPos getItemPos(Point point) const;
    TableView::ItemPos selectedItem() const;
    void selectItem(int c, int r);
    int selectedRow() const;
    void selectRow(int r);
    
    int newRow(ItemProperties itemProp);
    
    void setImageList(ImageList & imageList);
    void removeImageList();
    optional<ImageList> getImageList();
    
    void setGridLines(bool drawGrid);
    void setRowSelect(bool rowSelect);
    
    TableView::DrawItem onDrawItem(TableView::DrawItem callback);
    TableView::ItemEvent onClickItem(TableView::ItemEvent callback);
private:
    virtual optional<LRESULT> processNotification(UINT message, UINT notification, WPARAM wParam, LPARAM lParam);
    
    bool addItem(int r, ItemProperties itemProp);
    void setExtendedStyleFlag(long flag, bool value);
};

}

#endif
