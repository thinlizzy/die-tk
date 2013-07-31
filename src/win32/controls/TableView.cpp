#include "../../controls/TableView.h"
#include "TableViewWin32.h"

#define CONTROL_IMPL TableViewImpl
#include "ControlMacros.h"

namespace tk {

CONSTRUCTOR_IMPL(TableView);

int TableView::rows() const
{
    return IMPL.rows();
}

int TableView::columns() const
{
    return IMPL.columns();
}

void TableView::addColumn(ColumnProperties columnProp)
{
    IMPL.addColumn(columnProp);
}

void TableView::setColumn(int c, ColumnProperties columnProp)
{
    IMPL.setColumn(c,columnProp);
}

ColumnProperties TableView::column(int c) const
{
    return IMPL.column(c);
}

void TableView::deleteColumn(int c)
{
    IMPL.deleteColumn(c);
}

void TableView::setRows(int r)
{
    IMPL.setRows(r);
}

void TableView::deleteRow(int r)
{
    IMPL.deleteRow(r);
}

void TableView::setItem(int c, int r, ItemProperties itemProp)
{
    IMPL.setItem(c,r,itemProp);
}

void TableView::setItem(ItemPos pos, ItemProperties itemProp)
{
    IMPL.setItem(pos.c,pos.r,itemProp);
}

ItemProperties TableView::item(int c, int r) const
{
    return IMPL.item(c,r);
}

ItemProperties TableView::item(ItemPos pos) const
{
    return IMPL.item(pos.c,pos.r);
}

auto TableView::getItemPos(Point point) const -> ItemPos 
{
    return IMPL.getItemPos(point);
}

void TableView::setImageList(ImageList & imageList)
{
    IMPL.setImageList(imageList);
}

void TableView::removeImageList()
{
    IMPL.removeImageList();
}

optional<ImageList> TableView::getImageList()
{
    return IMPL.getImageList();
}

void TableView::setGridLines(bool drawGrid)
{
    IMPL.setGridLines(drawGrid);
}

void TableView::setRowSelect(bool rowSelect)
{
    IMPL.setRowSelect(rowSelect);
}

TableView::DrawItem TableView::onDrawItem(TableView::DrawItem callback)
{
    return IMPL.onDrawItem(callback);
}

TableView::ItemEvent TableView::onClickItem(TableView::ItemEvent callback)
{
    return IMPL.onClickItem(callback);
}

template<typename T>
void TableView::doAddRow(std::initializer_list<T> items)
{
    auto it = items.begin();
    int r = IMPL.newRow(*it);
    ++it;
    for( int c = 1; it != items.end(); ++it,++c ) {
        IMPL.setItem(c,r,*it);
    }
}

void TableView::addRow(std::initializer_list<die::NativeString> items)
{
    doAddRow(items);
}

void TableView::addRow(std::initializer_list<ItemProperties> items)
{
    doAddRow(items);
}

void TableView::setColumns(std::initializer_list<ColumnProperties> columnList)
{
    auto numCol = columns();        // save column count since we are changing it
    if( numCol >= columnList.size() ) {
        int c = 0;
        for( auto it = columnList.begin(); it != columnList.end(); ++it,++c ) {
            setColumn(c,*it);
        }
        for( ; c < numCol; ++c ) {
            deleteColumn(c);
        }
    } else {
        auto it = columnList.begin();
        for( int c = 0; c < numCol; ++c, ++it ) {
            setColumn(c,*it);
        }
        for( ; it != columnList.end(); ++it ) {
            addColumn(*it);
        }
    }
}

}

