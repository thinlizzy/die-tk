#ifndef DIE_TK_CUSTOM_LABEL_2020_28_06_H
#define DIE_TK_CUSTOM_LABEL_2020_28_06_H

#include "CustomControl.h"
#include "LabelImpl.h"

namespace tk {

class Label: public CustomControl {
	std::shared_ptr<LabelImpl> labelImpl;
public:
	Label(Surface & parent, ControlParams const & controlParams);

	LabelImpl & controlImpl() override;

	LabelImpl const & controlImpl() const override;

	void setText(NativeString const & text) override;

	NativeString text() const override;

	void setAutosize(bool enable);

	bool autosize() const;

	void setColor(RGBColor color);

	RGBColor color() const;

	void setBackgroundColor(optional<RGBColor> color);

	optional<RGBColor> backGroundColor() const;
private:
	bool redimAutosize();
};

}

#endif
