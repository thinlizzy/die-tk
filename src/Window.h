#ifndef WINDOW_H_jdfsr89f4wrjfdasfsd
#define WINDOW_H_jdfsr89f4wrjfdasfsd

#include "Control.h"
#include "Callbacks.h"
#include "WindowParams.h"
#include "SelectFileParams.h"
#include <vector>

namespace tk {

class WindowImpl;

class Window: public Control {
public:
    Window(WindowParams const & params = WindowParams());
	virtual ~Window();

	int state() const;
    
    void remove(Control & control);
    
    void addBorders() { setBorders(true); }
    void removeBorders() { setBorders(false); }
    void setBorders(bool value);
    
    die::NativeString selectFile(SelectFileParams const & params = SelectFileParams());
    std::vector<die::NativeString> selectFiles(SelectFileParams const & params = SelectFileParams());
    die::NativeString selectFileForSave(SelectFileParams const & params = SelectFileParams());
    
    using Control::onPaint;
    using Control::onKeyDown;
    using Control::onKeyUp;
    using Control::onKeypress;
	AllowOperation onClose(AllowOperation callback);
	ProcessResize onResize(ProcessResize callback);
	HandleEvent onUserEvent(HandleEvent callback);
    
    // internal use only
    WindowImpl & getImpl();     
    WindowImpl const & getImpl() const;
    std::shared_ptr<WindowImpl> getImplPtr();
protected:
    Window(std::shared_ptr<WindowImpl> impl);  // internal use only
};

}

#endif

