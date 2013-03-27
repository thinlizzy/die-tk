#ifndef MEMO_WIN32_jdff93r32rdfdl0222hdf
#define MEMO_WIN32_jdff93r32rdfdl0222hdf

#include "NativeControlWin32.h"

namespace tk {

class MemoImpl: public NativeControlImpl {
public:
	MemoImpl(Window & parent, ControlParams const & params);

	void setReadOnly(bool readOnly);
    
    virtual void setText(std::string const & text);
};

}

#endif
