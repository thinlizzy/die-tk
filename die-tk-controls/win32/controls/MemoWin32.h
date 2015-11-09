#ifndef MEMO_WIN32_jdff93r32rdfdl0222hdf
#define MEMO_WIN32_jdff93r32rdfdl0222hdf

#include "EditWin32.h"
#include "../components/NativeControlStringList.h"
#include "../../../win32/die-tk-win32.h"

namespace tk {

class MemoImpl: public EditImpl {
    NativeControlStringList linesList;
public:
	MemoImpl(HWND parentHwnd, ControlParams const & params);

    virtual MemoImpl * clone() const;

    virtual void setText(NativeString const & text);
    
    NativeControlStringList & lines();
    
    void scrollTo(unsigned lines);
};

}

#endif
