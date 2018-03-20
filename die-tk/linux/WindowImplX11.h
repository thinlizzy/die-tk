#ifndef WINDOWIMPLX11_H_DIE_TK_LINUX_DIEGO_2014_SEP_02
#define WINDOWIMPLX11_H_DIE_TK_LINUX_DIEGO_2014_SEP_02

#include <unordered_map>
#include <memory>
#include "NativeControlImpl.h"
#include "../src/WindowParams.h"

namespace tk {

class WindowImpl: public NativeControlImpl {
	typedef std::unordered_map<::Window,std::shared_ptr<NativeControlImpl>> Controls;
	Controls controls;
protected:
	WindowImpl() = default;
public:
	explicit WindowImpl(WindowParams const & params);
	~WindowImpl();

	int state() const;

    void setBorders(bool value);

	void registerControl(std::shared_ptr<NativeControlImpl> control);
    void unregisterControl(std::shared_ptr<NativeControlImpl> control);

	NativeString getText() const override;
	void setText(NativeString const & text) override;

	Rect rect() const override;

	void processMessage(XEvent & e) override;

	AllowOperation onClose(AllowOperation callback);
	ProcessResize onResize(ProcessResize callback);
	HandleResize afterResize(HandleResize callback);

	HandleEvent onUserEvent(HandleEvent callback);
private:
	void maximize(bool yes);
	bool maximized() const;
	WDims cachedDims;
	int ignoreConfigureNotify = 0;
};

}

#endif
