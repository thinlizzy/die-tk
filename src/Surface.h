#ifndef SURFACE_H_DIE_TK_2014_09_22_22_08
#define SURFACE_H_DIE_TK_2014_09_22_22_08

#include <memory>
#include "WindowObjects.h"
#include "Callbacks.h"

namespace tk {

class Canvas;
class NativeControlImpl;

/* base class for all widgets, including windows.
 */
class Surface {
protected:
    Surface() = default;
public:
    Surface(Surface const &) = default;
    Surface & operator=(Surface const &) = default;
    Surface(Surface &&) = default;
    Surface & operator=(Surface &&) = default;
    virtual ~Surface();

    explicit operator bool() const;

    int x() const;
    int y() const;
    Point pos() const;
    int width() const;
    int height() const;
    WDims dims() const;
    Rect rect() const;

    Surface & setPos(Point pos);
    Surface & setDims(WDims dims);
    Surface & setRect(Rect rect);

    die::NativeString getText() const;
    Surface & setText(die::NativeString const & text);

    ClipboardType copyToClipboard() const;

    void bringToFront();
    void sendToBack();

    bool enabled() const;
    void enable();
    void disable();
    void setEnabled(bool enabled);

    bool visible() const;
    void show();
    void hide();
    void setVisible(bool visible);

    Canvas & canvas();
    void repaint();

    void setCursor(Cursor cursor);
    void setBackground(RGBColor const & color);

    void clear(RGBColor const & color = RGBColor());

    Point screenToClient(Point const & point) const;

    HandleMouseButton onMouseDown(HandleMouseButton callback);
    HandleMouseButton onMouseUp(HandleMouseButton callback);
    HandleMouseMove onMouseEnter(HandleMouseMove callback);
    HandleMouseMove onMouseOver(HandleMouseMove callback);
    HandleMouseMove onMouseLeave(HandleMouseMove callback);
protected:
    std::shared_ptr<NativeControlImpl> impl;

    // most used callbacks
    ProcessKeyEvent onKeyDown(ProcessKeyEvent callback); // edit, memo
    ProcessKeyEvent onKeyUp(ProcessKeyEvent callback); // edit, memo
    ProcessKeypress onKeypress(ProcessKeypress callback); // edit, memo
    // seldom used callbacks
    HandlePaint onPaint(HandlePaint callback); // window, paintbox and custom controls
};

}

#endif

