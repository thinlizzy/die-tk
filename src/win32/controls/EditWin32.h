#ifndef EDIT_WIN32_H_jfhe3340394bmvbm4
#define EDIT_WIN32_H_jfhe3340394bmvbm4

#include "NativeControlWin32.h"

namespace tk {

class EditImpl: public NativeControlImpl {
public:
	EditImpl(Window & parent, ControlParams const & params);

	void setReadOnly(bool readOnly);
};

}

#endif
