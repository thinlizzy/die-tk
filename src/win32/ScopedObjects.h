#ifndef SCOPED_OBJECTS_H_jf43333333kfg
#define SCOPED_OBJECTS_H_jf43333333kfg

#include <windows.h>

namespace tk {

namespace scoped {

class DC {
	HWND hWnd;
public:
	HDC hdc;

	DC(HWND hWnd):
		hWnd(hWnd),
		hdc(GetDC(hWnd))
	{}

	DC(DC && temp):
		hWnd(temp.hWnd),
		hdc(temp.hdc)
	{
		temp.hdc = 0;
	}

	~DC() {
		if( hdc == 0 ) return;
		ReleaseDC(hWnd,hdc);
	}
};

inline PAINTSTRUCT BeginPaint2(HWND hWnd) { PAINTSTRUCT ps; BeginPaint(hWnd,&ps); return ps; }

class PaintSection {
	HWND hWnd;
public:
	PAINTSTRUCT ps;

	PaintSection(HWND hWnd):
		hWnd(hWnd),
		ps(BeginPaint2(hWnd))
	{
	}

	PaintSection(PaintSection && temp):
		hWnd(temp.hWnd),
		ps(temp.ps)
	{
		temp.ps.hdc = 0;
	}

	~PaintSection() {
		if( ps.hdc == 0 ) return;
		EndPaint(hWnd,&ps);
	}
};

template<typename H, typename Deleter>
class ScopedHandle {
	H h;
public:
	ScopedHandle(H h = 0): h(h) {}

	ScopedHandle(ScopedHandle && temp):
		h(temp.h)
	{
		temp.h = 0;
	}

	void reset(H h = 0) {
		release();
		this->h = h;
	}

	H get() const {
		return h;
	}

	~ScopedHandle() {
		release();
	}
private:
	void release() {
		if( h == 0 ) return;
		Deleter()(h);
		h = 0;
	}
};

class DeleterObject {
public:
    void operator()(HGDIOBJ h) { DeleteObject(h); }
};

typedef ScopedHandle<HBITMAP,DeleterObject> Bitmap;

typedef ScopedHandle<HBRUSH,DeleterObject> Brush;

}

}

#endif
