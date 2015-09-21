#ifndef PROPERTY_H_DIE_TK_LINUX_DIEGO_2015_SEP_19
#define PROPERTY_H_DIE_TK_LINUX_DIEGO_2015_SEP_19

#include <memory>
#include <X11/Xlib.h>

namespace tk {

class Property {
	struct XFreeFO {
		void operator()(unsigned char * ptr) { if( ptr ) XFree(ptr); }
	};
	std::unique_ptr<unsigned char, XFreeFO> data;
	int format;
	unsigned long nitems;
	Atom type;

	Atom * atomList() const;
public:
	Property() = default;
	Property(::Window windowId, Atom property);
	Property(Property &&) = default;
	explicit operator bool() const { return data.operator bool(); }
	Atom get(char const * name) const;
	bool hasItem(char const * name) const;

	size_t charSize() const;
	unsigned char const * charData() const;
};

} /* namespace tk */

#endif /* PROPERTY_H_ */
