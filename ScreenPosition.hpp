#ifndef SCREEN_POSITION_HPP_
#define SCREEN_POSITION_HPP_

struct ScreenPosition {
public:
    ScreenPosition(int x, int y)
    : x(x)
    , y(y)
    {}
    ScreenPosition() : ScreenPosition(0, 0) {}
    ~ScreenPosition() {}
    void Set(int xv, int yv) {
        x = xv;
        y = yv;
    }

public:
    int x;
    int y;
}; // SCREEN_POSITION_HPP_

#endif // SCREEN_POSITION_HPP_
