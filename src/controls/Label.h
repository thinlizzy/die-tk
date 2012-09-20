#ifndef LABEL_H_hfjds34230pp2b4jugf
#define LABEL_H_hfjds34230pp2b4jugf

#include "Control.h"
#include <string>

namespace tk {

class Label: public virtual Control {
public:
	virtual std::string getText() const = 0;
	virtual void setText(std::string const & text) = 0;

	virtual void setAutosize(bool autosize) = 0;
};

}


#endif
