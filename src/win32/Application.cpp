#undef __STRICT_ANSI__
#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#include <iostream>

#include "../Application.h"
#include "ResourceManager.h"
#include "ConvertersWin32.h"

#include "../log.h"

namespace tk {

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

    auto lStdHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    auto hConHandle = _open_osfhandle(intptr_t(lStdHandle), _O_TEXT);

    FILE * fp = _fdopen( hConHandle, "w" );
    *stdout = *fp;
    setvbuf( stdout, NULL, _IONBF, 0 );

    // redirect unbuffered STDIN to the console
    lStdHandle = GetStdHandle(STD_INPUT_HANDLE);
    hConHandle = _open_osfhandle(intptr_t(lStdHandle), _O_TEXT);
    fp = _fdopen( hConHandle, "r" );
    *stdin = *fp;
    setvbuf( stdin, NULL, _IONBF, 0 );

    // redirect unbuffered STDERR to the console
    lStdHandle = GetStdHandle(STD_ERROR_HANDLE);
    hConHandle = _open_osfhandle(intptr_t(lStdHandle), _O_TEXT);
    fp = _fdopen( hConHandle, "w" );
    *stderr = *fp;
    setvbuf( stderr, NULL, _IONBF, 0 );

    // make cout, wcout, cin, wcin, wcerr, cerr, wclog and clog
    // point to console as well
    std::ios::sync_with_stdio();
}

void Application::waitForMessages()
{
	WaitMessage();
}

void Application::processMessages()
{
    resourceManager.processMessages();
}

Point Application::getCursorPos() const
{
    POINT p;
    GetCursorPos(&p);
    return convertPoint(p);
}

NativeString Application::getClipboardText()
{
    NativeString result;
    if( ! IsClipboardFormatAvailable(CF_UNICODETEXT) ) return result;
        
    if( ! OpenClipboard(NULL) ) {
        log::error("OpenClipboard failed");
        return result;
    }
    
    HGLOBAL hglb = GetClipboardData(CF_UNICODETEXT);
    if( hglb != NULL ) {
        auto lptstr = reinterpret_cast<wchar_t *>(GlobalLock(hglb));
        if( lptstr != NULL ) {
            result.wstr = lptstr;
            GlobalUnlock(hglb); 
        }
    }
    
    CloseClipboard();
    return result;
}

}
