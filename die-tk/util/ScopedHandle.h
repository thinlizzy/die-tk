#ifndef SCOPED_HANDLE_H_DIE_TK_2015_03_20
#define SCOPED_HANDLE_H_DIE_TK_2015_03_20

#include <utility>

namespace tk {

namespace scoped {

template<typename H, typename Deleter>
class Handle {
	H h;
public:
	Handle(H h = H()): h(h) {}

	Handle(Handle && temp):
		h(temp.h)
	{
		temp.h = H();
	}

    ~Handle() {
		dispose();
	}

	Handle & operator=(Handle other)
	{
        swap(other);
        return *this;
	}

    void swap(Handle & other)
    {
        std::swap(h,other.h);
    }

	void reset(H h = H()) {
		dispose();
		this->h = h;
	}

	H get() const {
		return h;
	}

	H release() {
		H result = h;
        h = H();
        return result;
	}

    explicit operator bool() const {
        return h != H();
    }
private:
	void dispose() {
		if( h == H() ) return;
		Deleter()(h);
		h = H();
	}
};

}

}

#endif
