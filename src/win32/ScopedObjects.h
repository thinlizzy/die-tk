#ifndef SCOPED_OBJECTS_H_jf43333333kfg
#define SCOPED_OBJECTS_H_jf43333333kfg

#include <windows.h>
#include <utility>

namespace tk {

namespace scoped {

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
    
    DC & operator=(DC other)
    {
        swap(other);
        return *this;
    }
    
    void swap(DC & other)
    {
        std::swap(hdc,other.hdc);
        std::swap(hWnd,other.hWnd);
    }

	~DC() {
		if( hdc == 0 ) return;
		ReleaseDC(hWnd,hdc);
	}
};

class BitmapDC {
    HGDIOBJ old;
public:
    HDC hdc;
    HBITMAP hbmp;
    
    BitmapDC(HDC hdc_, HBITMAP hbmp):
        hdc(CreateCompatibleDC(hdc_)),
        hbmp(hbmp)
    {
    }
    
    BitmapDC(BitmapDC && other):
        old(other.old),
        hdc(other.hdc),
        hbmp(other.hbmp)
    {
        other.hdc = 0;
    }
    
    BitmapDC & operator=(BitmapDC other)
    {
        swap(other);
        return *this;
    }
    
    void swap(BitmapDC & other)
    {
        std::swap(hdc,other.hdc);
        std::swap(hbmp,other.hbmp);
        std::swap(old,other.old);
    }
    
    // TODO return a guard object to unselect
    void select()
    {
        old = SelectObject(hdc,hbmp);
    }
        
    void unselect()
    {
        SelectObject(hdc,old);
    }
        
    virtual ~BitmapDC()
    {
        if( hdc == 0 ) return;

        DeleteObject(hbmp);
        DeleteDC(hdc);    
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

    ~ScopedHandle() {
		dispose();
	}
    
	ScopedHandle & operator=(ScopedHandle other)
	{
        swap(other);
        return *this;
	}

    void swap(ScopedHandle & other)
    {
        std::swap(h,other.h);
    }

	void reset(H h = 0) {
		dispose();
		this->h = h;
	}

	H get() const {
		return h;
	}
    
	H release() {
		H result = h;
        h = 0;
        return result;
	}
    
    explicit operator bool() const {
        return h != 0;
    }
private:
	void dispose() {
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

class DeleterCompatDC {
public:
    void operator()(HDC hdc) { DeleteDC(hdc); }
};

typedef ScopedHandle<HDC,scoped::DeleterCompatDC> CompatibleDC;

}

}

#endif
