#ifndef APPLICATION_H_fdj498gj454328gfjh
#define APPLICATION_H_fdj498gj454328gfjh

#include <memory>
#include <NativeString.h>
#include "WindowObjects.h"

namespace tk {

class Application {
public:
	void processMessages();
	void waitForMessages();

    void showConsole();

    Point getCursorPos() const;
    
    void showMessage(die::NativeString const & message);
};

}

#endif
