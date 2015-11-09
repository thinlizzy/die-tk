#ifndef EDIT_WIN32_H_jfhe3340394bmvbm4
#define EDIT_WIN32_H_jfhe3340394bmvbm4

#include "../../../win32/die-tk-win32.h"

namespace tk {

class EditImpl: public NativeControlImpl {
public:
	EditImpl(HWND parentHwnd, ControlParams const & params);

    virtual EditImpl * clone() const;

	void setReadOnly(bool readOnly);
    void setTextAlign(HTextAlign alignment);
    
    bool readOnly() const;
protected:
	EditImpl(HWND parentHwnd, ControlParams const & params, DWORD style);
    void cloneInto(EditImpl & result) const;
};

}

#endif
