#ifndef MEMO_H_kjfdsfk34
#define MEMO_H_kjfdsfk34

#include "Control.h"
#include <string>

namespace tk {

class Memo: public virtual Control {
public:
	virtual std::string getText() const = 0;
	virtual void setText(std::string const & text) = 0;

	virtual void setReadOnly(bool readOnly) = 0;
};

}

#endif
