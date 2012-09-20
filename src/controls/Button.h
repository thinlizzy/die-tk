#ifndef BUTTON_H_gjhrjh4543jfdsgjdsio23
#define BUTTON_H_gjhrjh4543jfdsgjdsio23

#include "Control.h"
#include <string>

namespace tk {

class Button: public virtual Control {
public:
	virtual std::string getText() const = 0;
	virtual void setText(std::string const & text) = 0;
};

}

#endif
