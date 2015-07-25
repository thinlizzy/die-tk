#ifndef APPLICATION_H_fdj498gj454328gfjh
#define APPLICATION_H_fdj498gj454328gfjh

#include <memory>
#include "NativeString.h"
#include "objects/Point.h"

namespace tk {

class Application {
public:
	void processMessages();
	void waitForMessages();

    void showConsole();

    void showMessage(NativeString const & message);
    
    Point getCursorPos() const;
    
    NativeString getClipboardText();
};

}

#endif
