#include "../src/Application.h"
#include "../src/Callbacks.h"
#include "../src/Window.h"
#include "../src/Canvas.h"

#include "../die-tk-extra/die-tk-extra.h"
#include "../../libimage/src/image.h"
#include <fileutils.h>

#include <iostream>
#include <fstream>

int main()
{
	{
		using namespace tk;
		Application app;

		app.showConsole();

		Window window1;
		window1.onClose([]() -> bool {
			std::cout << "window was closed!" << std::endl;
			return true;
		});

		Window window2(WindowParams().text("second window"));
		window2.clear(RGBColor(100,0,0));
		window2.onClose([]() -> bool {
			static bool x = false;
			if( x ) {
				std::cout << "second window was closed!" << std::endl;
				return true;
			}
			x = true;
			std::cout << "try again!" << std::endl;
			return false;
		});

		img::Image image("DIEGO1.jpg");

		window1.canvas().setBrush(tk::RGBColor(0,100,0));
		window1.canvas().setPen(tk::RGBColor(0,100,0));
		window1.onPaint([&](Canvas & canvas, Rect rect) {
			canvas.fillRect(rect);
		});
		Button button1(window1,ControlParams().text("click me").start(20,20));
        button1.onClick([]() {
			std::cout << "clicked me" << std::endl;
		});

		button1.onMouse([](MouseEvent evt, Point pt) {
			std::cout << "clicked me with " << evt.type << " at " << pt << std::endl;
		});

		PaintBox paintboxG(window2,ControlParams().start(window2.width()/2,window2.height()/2).dims(100,100));
		paintboxG.setBackground(tk::RGBColor(200,100,0));

		Image imageCtl(window2,ControlParams().start(300,100));
		imageCtl.setImage(image.rawBits(),image.getWindowSystemHeader());

		PaintBox paintboxB(window2,ControlParams().start(280,80).dims(100,100));
		paintboxB.setBackground(tk::RGBColor(0,100,200));

		Button buttonLeft(window2,ControlParams().text("left!").start(260,130));
		Button buttonRight(window2,ControlParams().text("right!").start(900,130));

        paintboxG.bringToFront();      // that one is hidden by the image control
        paintboxB.bringToFront();      // that is partially hidden

        buttonRight.bringToFront();

        ComboBox combobox(window2,ControlParams().start(10,10));
        combobox.setVisibleItems(5);
        combobox.addString("laranja");
        combobox.addString("banana");
        combobox.addString("maca");
        combobox.addString("uva");
        combobox.addString("bergamota");
        combobox.addString("morango");
        combobox.addString("maracujeba");
        combobox.addString("apricu");
        
        combobox.onChange([&](){
			std::cout << "chose " << combobox.getString(combobox.selectedIndex()) << std::endl;
        });

        CheckBox checkbox(window2,ControlParams().start(100,100).text("disable box").autosize());
        checkbox.onClick([&](){
			combobox.setEnabled(! checkbox.checked());
        });

        ImageList imageList(WDims(16,16));
        auto iBubble = addFile(imageList,"bubble.png");
        auto iFolder = addFile(imageList,"folder.png");
        
        TreeView treeView(window1,ControlParams().start(10,100).dims(100,300));
        treeView.setImageList(imageList);
        auto root = treeView.root();
        auto it1 = root.addChild(ItemProperties().setText("test").setImageIndex(iFolder));
        auto item1 = *it1;
        item1.addChild(ItemProperties().setText("child").setImageIndex(iBubble));
        item1.addChild(ItemProperties().setText("child2"));
        root.addChild(ItemProperties().setText("second item").setImageIndex(iFolder));
        root.addChild(ItemProperties().setText("third item").setImageIndex(iFolder));
        
        treeView.onChange([](TreeView::Item item){
			std::cout << "mudou para " << item.getProperties().text.toUTF8() << std::endl;
        });
        
        treeView.beforeChange([](TreeView::Item before, TreeView::Item after) -> bool {
            return after.getProperties().text != "second item";
        });
        
        treeView.beforeCollapse([](TreeView::Item item) -> bool {
            std::cout << "vai encolher " << item.getProperties().text.toUTF8() << std::endl;
            return true;
        });
        
        
        TreeView treeView2(window1,ControlParams().start(treeView.pos().addX(150)).dims(300,300));
        treeView2.setImageList(imageList);
        FileTreeView ftv(treeView2);
        ftv.setBaseDir("c:\\");
        
        Memo text(window1,ControlParams().start(500,100).dims(300,390));
        
        TableView tableView(window1,ControlParams().start(810,100).dims(300,200));
        tableView.addColumn(ColumnProperties("column 1",100));
        tableView.addColumn(ColumnProperties("column 2"));
        tableView.addColumn(ColumnProperties("column 3"));
        tableView.setColumn(1,ColumnProperties("second",30));
        
        auto colProp = tableView.column(2);
        std::cout << "column 2 width[" << colProp.width << "] header [" << colProp.header << "]" << std::endl;
        
        tableView.setImageList(imageList);
        tableView.addColumn(ColumnProperties("col image",100,1));
        tableView.addRow({die::NativeString("test")});
        tableView.addRow({die::NativeString("1"),die::NativeString("2"),die::NativeString("3")});
        tableView.addRow({ItemProperties().setText("image").setImageIndex(1)});
        tableView.setItem(3,0,ItemProperties().setText("changed").setImageIndex(0));
        
        Label tvLabelRows(window1,ControlParams().text("set rows").start(tableView.rect().posDown(2)).autosize());
        Edit tvEditRows(window1,ControlParams().start(tvLabelRows.rect().posRight(2)).dims(30,20));
        Button tvButRows(window1,ControlParams().text("OK").start(tvEditRows.rect().posRight(2)).autosize());
        tvButRows.onClick([&](){
            tableView.setRows(tvEditRows.getText().toInt());
        });
        
        Label tvLabelDelRow(window1,ControlParams().text("delete row").start(tvLabelRows.rect().posDown(2)).autosize());
        Edit tvEditDelRow(window1,ControlParams().start(tvLabelDelRow.rect().posRight(2)).dims(30,20));
        Button tvButDelRow(window1,ControlParams().text("OK").start(tvEditDelRow.rect().posRight(2)).autosize());
        tvButDelRow.onClick([&](){
            tableView.deleteRow(tvEditDelRow.getText().toInt());
        });
        
        bool hasGrid = true;
        Button tvToggleGrid(window1,ControlParams().text("toggle grid lines").start(tvLabelDelRow.rect().posDown(2)).autosize());
        tvToggleGrid.onClick([&](){
            hasGrid = ! hasGrid;
            tableView.setGridLines(hasGrid);
        });
        
        Button buttonFileTreeView(window1,ControlParams()
            .start(treeView2.pos().addY(treeView2.height()+5))
            .autosize()
            .text("open"));
        buttonFileTreeView.onClick([&](){
            std::cout << "opening filename = " << ftv.getFile().filename << ' ' <<
                    "full path = " << ftv.getFile().fullPath << ' ' <<
                    "isDir? = " << ftv.getFile().isDirectory << std::endl;
            if( ftv.getFile().isDirectory ) return;
            
            fs::FileStreamWrapper file(ftv.getFile().fullPath);
            if( file ) {
                std::string buf(2048,'\0');
                file.read(&buf[0],buf.size());
                buf.resize(file.gcount());
                text.setText(buf);
            } else {
                std::cout << "fail!" << std::endl;
                app.showMessage("failed to open " + ftv.getFile().fullPath);
            }
        });
        
		PaintBox imagepb(window1,ControlParams().start(500,500).dims(200,200));
		imagepb.onPaint([&](Canvas & canvas, Rect rect) {
			canvas.drawImage(ImageRef::native(image.getWindowSystemHeader(),image.rawBits()));
		});
        
        
		while( window1.state() == ws_visible || window2.state() == ws_visible ) {
			app.waitForMessages();
			app.processMessages();
		}
	}

	std::cout << "type something and press enter to quit ";
	int a;
	std::cin >> a;
}
