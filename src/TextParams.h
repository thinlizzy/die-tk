#ifndef TEXTPARAMS_H_DIE_TK_2014_03_22_11_26
#define	TEXTPARAMS_H_DIE_TK_2014_03_22_11_26

#include "objects/Color.h"

namespace tk {
    
enum HTextAlign { hta_left, hta_right, hta_center };
enum VTextAlign { vta_top, vta_bottom, vta_center };

class TextParams {
public:
	HTextAlign h_align;
	VTextAlign v_align;
	RGBColor textColor,backgroundColor;

	TextParams():
		h_align(hta_left),
		v_align(vta_center),
		textColor(0,0,0),
		backgroundColor(255,255,255)
	{}
	TextParams & horizontalAlign(HTextAlign align) { h_align = align; return *this; }
	TextParams & verticalAlign(VTextAlign align) { v_align = align; return *this; }
	TextParams & color(RGBColor const & color) { textColor = color; return *this; }
	TextParams & background(RGBColor const & color) { backgroundColor = color; return *this; }
};

}

#endif
