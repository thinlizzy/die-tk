#ifndef LABEL_WIN_32_H_jjjjjjjj33333hfdsfn34jjjjj43ka33fdf
#define LABEL_WIN_32_H_jjjjjjjj33333hfdsfn34jjjjj43ka33fdf

#include "NativeControlWin32.h"

namespace tk {

class LabelImpl: public NativeControlImpl {
	bool autosize;
public:
	LabelImpl(Window & parent, ControlParams const & params);

	void setAutosize(bool autosize);
private:
    void doAutosize();
};

}

#endif
