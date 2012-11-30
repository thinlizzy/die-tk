#ifndef APPLICATION_H_fdj498gj454328gfjh
#define APPLICATION_H_fdj498gj454328gfjh

#include <memory>
#include "Window.h"
#include "Callbacks.h"
#include "WindowParams.h"
#include "ControlParams.h"

#include "controls/Control.h"
#include "controls/Button.h"
#include "controls/Image.h"
#include "controls/ComboBox.h"
#include "controls/Edit.h"
#include "controls/Memo.h"
#include "controls/CheckBox.h"
#include "controls/Label.h"
#include "controls/TreeView.h"

#include "components/ImageList.h"

namespace tk {

typedef std::shared_ptr<Window> WindowPtr;

class ApplicationImpl;

class Application {
	std::shared_ptr<ApplicationImpl> appImpl;
public:
	Application();
	~Application();
	void processMessages();
	void waitForMessages();

	WindowPtr createWindow(WindowParams const & params = WindowParams());

	std::shared_ptr<Control> createPaintBox(WindowPtr parent, ControlParams const & params = ControlParams());
	std::shared_ptr<Button> createButton(WindowPtr parent, ControlParams const & params = ControlParams());
	std::shared_ptr<Image> createImage(WindowPtr parent, ControlParams const & params = ControlParams());
	std::shared_ptr<ComboBox> createComboBox(WindowPtr parent, ControlParams const & params = ControlParams());
	std::shared_ptr<Edit> createEdit(WindowPtr parent, ControlParams const & params = ControlParams());
	std::shared_ptr<Memo> createMemo(WindowPtr parent, ControlParams const & params = ControlParams());
	std::shared_ptr<CheckBox> createCheckBox(WindowPtr parent, ControlParams const & params = ControlParams());
	std::shared_ptr<Label> createLabel(WindowPtr parent, ControlParams const & params = ControlParams());
    std::shared_ptr<TreeView> createTreeView(WindowPtr parent, ControlParams const & params = ControlParams());

	void removeControl(std::shared_ptr<Control> control);

	std::shared_ptr<ImageList> createImageList(WDims dims, int capacity = 10);

	void onPaint(std::shared_ptr<Control> control, HandlePaint callback);
	void onMouse(std::shared_ptr<Control> control, HandleMouseEvent callback);
	void onKey(std::shared_ptr<Control> control, ProcessKeyEvent callback);
	void onKeypress(std::shared_ptr<Control> control, ProcessKeypress callback);
	void onClick(std::shared_ptr<Control> control, HandleOperation callback);
	void onChange(std::shared_ptr<Control> control, HandleOperation callback);

	void onClose(WindowPtr window, AllowOperation callback);
	void onResize(WindowPtr window, ProcessResize callback);
	void onUserEvent(WindowPtr window, HandleEvent callback);

    void showConsole();

    Point getCursorPos() const;
private:
	template<typename C, typename CI>
	std::shared_ptr<C> create(WindowPtr parent, ControlParams const & params);
};

}

#endif
