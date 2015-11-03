#include "Property.h"
#include <cstring>
#include <algorithm>
#include "ResourceManager.h"
#include "../src/log.h"

namespace tk {

Property::Property(::Window windowId, Atom property)
{
	unsigned long bytes_after;
	unsigned char * ret = 0;

	int read_bytes = 1024;

	//Keep trying to read the property until there are no bytes unread.
	do {
		if( ret != 0 ) {
			XFree(ret);
		}
		XGetWindowProperty(resourceManager.dpy, windowId, property, 0, read_bytes, False, AnyPropertyType,
							&type, &format, &nitems, &bytes_after,
							&ret);

		read_bytes *= 2;
	} while( bytes_after != 0 );
	data.reset(ret);

	/*
	log::info("Actual type: ",resourceManager.getAtomName(type));
	log::info("Actual format: ",format);
	log::info("Number of items: ",nitems);
	for( int i=0; i < nitems; i++ ) {
		auto atomName = resourceManager.getAtomName(atomList()[i]);
		log::info("Item ",i," = ",atomName);
	}
	*/
}

Atom * Property::atomList() const
{
	return reinterpret_cast<Atom *>(data.get());
}

size_t Property::charSize() const
{
	return nitems * (format/8);  // perhaps need to pass 8 as argument?
}

unsigned char const * Property::charData() const
{
	return data.get();
}

Atom Property::get(char const * name) const
{
	auto end = atomList()+nitems;
	auto it = std::find_if(atomList(),end,[name](Atom atom) {
		auto atomName = resourceManager.getAtomName(atom);
		return std::strcmp(name,atomName) == 0;
	});
	return it == end ? None : *it;
}

bool Property::hasItem(char const * name) const
{
	return get(name) != None;
}

} /* namespace tk */
