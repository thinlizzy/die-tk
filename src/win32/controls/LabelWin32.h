#ifndef LABEL_WIN_32_H_jjjjjjjj33333hfdsfn34jjjjj43ka33fdf
#define LABEL_WIN_32_H_jjjjjjjj33333hfdsfn34jjjjj43ka33fdf

#include "NativeControlWin32.h"
#include "../../controls/Label.h"

#pragma warning( push )
#pragma warning( disable : 4250 )

namespace tk {

class LabelImpl: public NativeControlImpl, public Label {
	bool autosize;
public:
	LabelImpl(HWND parent_hWnd, ControlParams const & params);

	virtual std::string getText() const;
	virtual void setText(std::string const & text);

	virtual void setAutosize(bool autosize);
private:
    void doAutosize();
};

}

#pragma warning( pop )


#endif
