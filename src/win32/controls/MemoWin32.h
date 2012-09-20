#ifndef MEMO_WIN32_jdff93r32rdfdl0222hdf
#define MEMO_WIN32_jdff93r32rdfdl0222hdf

#include "NativeControlWin32.h"
#include "../../controls/Memo.h"

#pragma warning( push )
#pragma warning( disable : 4250 )

namespace tk {

class MemoImpl: public NativeControlImpl, public Memo {
public:
	MemoImpl(HWND parent_hWnd, ControlParams const & params);

	virtual std::string getText() const;
	virtual void setText(std::string const & text);

	virtual void setReadOnly(bool readOnly);
};

}

#pragma warning( pop )

#endif
