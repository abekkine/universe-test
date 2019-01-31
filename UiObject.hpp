#ifndef UI_OBJECT_HPP_
#define UI_OBJECT_HPP_

#include "ScreenPosition.hpp"

class UiObject {
public:
    virtual ~UiObject() {}
    virtual void SetPosition(const ScreenPosition & p) {
        x_ = p.x;
        y_ = p.y;
    }
    virtual void SetSize(const int w, const int h) {
        w_ = w;
        h_ = h;
    }

protected:
    virtual bool isMouseOver(const ScreenPosition & p) {
        return (
            p.y > y_ &&
            p.y < y_ + h_ &&
            p.x > x_ &&
            p.x < x_ + w_ );
    }

protected:
    int x_, y_;
    int w_, h_;
};

#endif // UI_OBJECT_HPP_
