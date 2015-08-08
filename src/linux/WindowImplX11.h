#ifndef WINDOWIMPLX11_H_DIE_TK_LINUX_DIEGO_2014_SEP_02
#define WINDOWIMPLX11_H_DIE_TK_LINUX_DIEGO_2014_SEP_02

#include "../WindowParams.h"
#include "CanvasX11.h"
#include <unordered_map>
#include <memory>
#include "NativeControlImpl.h"

namespace tk {

class WindowImpl: public NativeControlImpl {
	typedef std::unordered_map<::Window,std::shared_ptr<NativeControlImpl>> Controls;
	Controls controls;
	CanvasX11 windowCanvas;
protected:
	WindowImpl() = default;
public:
	static ::Window createWindow(int x, int y, int width, int height);

	explicit WindowImpl(WindowParams const & params);
	~WindowImpl();

	int state() const;

    void setBorders(bool value);

	void registerControl(std::shared_ptr<NativeControlImpl> control);
    void unregisterControl(std::shared_ptr<NativeControlImpl> control);

	Rect rect() const override;

	NativeString getText() const override;
	void setText(NativeString const & text) override;

	Canvas & canvas() override;

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
