#include "NativeControlWin32.h"
#include "../../controls/Edit.h"

#pragma warning( push )
#pragma warning( disable : 4250 )

namespace tk {

class EditImpl: public NativeControlImpl, public Edit {
public:
	EditImpl(HWND parent_hWnd, ControlParams const & params);

	virtual std::string getText() const;
	virtual void setText(std::string const & text);

	virtual void setReadOnly(bool readOnly);
};

}

#pragma warning( pop )
