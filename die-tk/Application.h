#ifndef APPLICATION_H_fdj498gj454328gfjh
#define APPLICATION_H_fdj498gj454328gfjh

#include "NativeString.h"
#include "objects/Point.h"

namespace tk {

class Application {
public:
	void processMessages();
	void waitForMessages();

    void showConsole();

    Point getCursorPos() const;
    
    NativeString getClipboardText();
};

}

#endif
