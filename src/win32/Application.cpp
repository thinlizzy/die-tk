#undef __STRICT_ANSI__
#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#include <iostream>

#include "../Application.h"
#include "../trace.h"

#include "ApplicationWin32.h"

namespace tk {

Application::Application():
	appImpl(globalAppImpl)
{}

Application::~Application()
{}

WindowPtr Application::createWindow(WindowParams const & params)
{
	auto windowImpl = std::make_shared<WindowImpl>(params);
	appImpl->registerWindow(windowImpl);
	return std::dynamic_pointer_cast<Window>(windowImpl);
}

template<typename C, typename CI>
std::shared_ptr<C> Application::create(WindowPtr parent, ControlParams const & params)
{
	auto parentWindowImpl = std::dynamic_pointer_cast<WindowImpl>(parent);
	auto controlImpl = std::make_shared<CI>(parentWindowImpl->hWnd,params);
	appImpl->registerControl(controlImpl);
	parentWindowImpl->registerControl(controlImpl);
	return std::dynamic_pointer_cast<C>(controlImpl);
}

std::shared_ptr<PaintBox> Application::createPaintBox(WindowPtr parent, ControlParams const & params)
{
	return create<PaintBox,PaintBoxImpl>(parent,params);
}

std::shared_ptr<Button> Application::createButton(WindowPtr parent, ControlParams const & params)
{
	return create<Button,ButtonImpl>(parent,params);
}

std::shared_ptr<Image> Application::createImage(WindowPtr parent, ControlParams const & params)
{
	return create<Image,ImageImpl>(parent,params);
}

std::shared_ptr<ComboBox> Application::createComboBox(WindowPtr parent, ControlParams const & params)
{
	return create<ComboBox,ComboBoxImpl>(parent,params);
}

std::shared_ptr<Edit> Application::createEdit(WindowPtr parent, ControlParams const & params)
{
	return create<Edit,EditImpl>(parent,params);
}

std::shared_ptr<Memo> Application::createMemo(WindowPtr parent, ControlParams const & params)
{
	return create<Memo,MemoImpl>(parent,params);
}

std::shared_ptr<CheckBox> Application::createCheckBox(WindowPtr parent, ControlParams const & params)
{
	return create<CheckBox,CheckBoxImpl>(parent,params);
}

std::shared_ptr<Label> Application::createLabel(WindowPtr parent, ControlParams const & params)
{
	return create<Label,LabelImpl>(parent,params);
}

std::shared_ptr<TreeView> Application::createTreeView(WindowPtr parent, ControlParams const & params)
{
	return create<TreeView,TreeViewImpl>(parent,params);
}



std::shared_ptr<ImageList> Application::createImageList(WDims dims, int capacity)
{
    return std::make_shared<ImageListImpl>(dims,capacity);
}



void Application::waitForMessages()
{
	WaitMessage();
}


void Application::showConsole()
{
    if( ! AllocConsole() ) {
        return;
    }

    // set the screen buffer to be big enough to let us scroll text
    CONSOLE_SCREEN_BUFFER_INFO coninfo;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE),&coninfo);
    coninfo.dwSize.Y = 500;
    SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE),coninfo.dwSize);

    // redirect unbuffered STDOUT to the console

    long lStdHandle = long(GetStdHandle(STD_OUTPUT_HANDLE));
    int hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);

    FILE * fp = _fdopen( hConHandle, "w" );
    *stdout = *fp;
    setvbuf( stdout, NULL, _IONBF, 0 );

    // redirect unbuffered STDIN to the console
    lStdHandle = (long)GetStdHandle(STD_INPUT_HANDLE);
    hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
    fp = _fdopen( hConHandle, "r" );
    *stdin = *fp;
    setvbuf( stdin, NULL, _IONBF, 0 );

    // redirect unbuffered STDERR to the console
    lStdHandle = (long)GetStdHandle(STD_ERROR_HANDLE);
    hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
    fp = _fdopen( hConHandle, "w" );
    *stderr = *fp;
    setvbuf( stderr, NULL, _IONBF, 0 );

    // make cout, wcout, cin, wcin, wcerr, cerr, wclog and clog
    // point to console as well
    std::ios::sync_with_stdio();
}

void Application::processMessages()
{
    appImpl->processMessages();
}

Point Application::getCursorPos() const
{
    POINT p;
    GetCursorPos(&p);
    return convertPoint(p);
}


#define CHKNULL(name,elem) \
	if( callback == nullptr ) { \
		auto it = appImpl->name.find(elem); \
		if( it != appImpl->name.end() ) appImpl->name.erase(it); \
		return; \
	}

#define ONCTL(name,type) \
void Application::name(std::shared_ptr<Control> control, type callback) \
{ \
	CHKNULL(name,control) \
	appImpl->name[control] = callback; \
}

#define ONWND(name,type) \
void Application::name(WindowPtr window, type callback) \
{ \
	CHKNULL(name,window) \
	appImpl->name[window] = callback; \
}

ONCTL(onPaint,HandlePaint)
ONCTL(onMouse,HandleMouseEvent)
ONCTL(onKey,ProcessKeyEvent)
ONCTL(onKeypress,ProcessKeypress)
ONCTL(onClick,HandleOperation)
ONCTL(onChange,HandleOperation)

ONWND(onClose,AllowOperation)
ONWND(onResize,ProcessResize)
ONWND(onUserEvent,HandleEvent)

}
