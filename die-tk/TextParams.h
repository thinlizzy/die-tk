#ifndef TEXTPARAMS_H_DIE_TK_2014_03_22_11_26
#define	TEXTPARAMS_H_DIE_TK_2014_03_22_11_26

#include "objects/Color.h"
#include "util/optional.h"

namespace tk {
    
enum class HTextAlign { left, right, center };
enum class VTextAlign { top, bottom, center };

class TextParams {
public:
	HTextAlign h_align;
	VTextAlign v_align;
	RGBColor textColor;
	optional<RGBColor> backgroundColor;

	TextParams():
		h_align(HTextAlign::left),
		v_align(VTextAlign::center),
		textColor(0,0,0),
		backgroundColor({255,255,255})
	{}
	TextParams & horizontalAlign(HTextAlign align) { h_align = align; return *this; }
	TextParams & verticalAlign(VTextAlign align) { v_align = align; return *this; }
	TextParams & color(RGBColor const & color) { textColor = color; return *this; }
	TextParams & background(optional<RGBColor> color) { backgroundColor = color; return *this; }
};

}

#endif
