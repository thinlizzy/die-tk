#ifndef WINDOWIMPLX11_H_DIE_TK_LINUX_DIEGO_2014_SEP_02
#define WINDOWIMPLX11_H_DIE_TK_LINUX_DIEGO_2014_SEP_02

#include "../WindowParams.h"
#include "../SelectFileParams.h"
#include "WindowSurfaceX11.h"

namespace tk {

class WindowImpl: public NativeControlImpl {
public:
	WindowImpl(WindowParams const & params);
	~WindowImpl();

	int state() const;

    void setBorders(bool value);

    //

    die::NativeString selectFile(SelectFileParams const & params = SelectFileParams());
    std::vector<die::NativeString> selectFiles(SelectFileParams const & params = SelectFileParams());
    die::NativeString selectFileForSave(SelectFileParams const & params = SelectFileParams());

	void registerControl(std::shared_ptr<NativeControlImpl> control);
    void unregisterControl(std::shared_ptr<NativeControlImpl> control);

    //

	void processMessage(XEvent & e) override;

	AllowOperation onClose(AllowOperation callback);
	ProcessResize onResize(ProcessResize callback);
	HandleEvent onUserEvent(HandleEvent callback);

private:
	void maximize(bool yes);
	WDims cachedDims;
};

}

#endif
