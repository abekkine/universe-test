#ifndef UI_SLIDER_HPP_
#define UI_SLIDER_HPP_

#include "UiObject.hpp"

class UiSlider : public UiObject {
private:
    const double kTopValue;
    const double kBottomValue;

public:
    UiSlider()
    : kTopValue(1.0)
    , kBottomValue(0.0)
    {
        position_ = 0;
        UpdateCurrentValue();
        value_callback_ = 0;
        active_ = false;
    }
    ~UiSlider() {}
    void SetValueCallback(std::function<void(const double &)> callback) {
        value_callback_ = callback;
    }
    void Render() {
        int sm = 4;

        glColor3f(1.0, 1.0, 1.0);
        glLineWidth(2.0);
        glBegin(GL_LINE_LOOP);
        glVertex2i(x_ - sm, y_ - sm);
        glVertex2i(x_ + w_ + sm, y_ - sm);
        glVertex2i(x_ + w_ + sm, y_ + h_ + sm);
        glVertex2i(x_ - sm, y_ + h_ + sm);
        glEnd();

        glColor3f(1.0, 0.0, 0.0);
        glLineWidth(4.0);
        glBegin(GL_LINES);
        glVertex2i(x_, position_);
        glVertex2i(x_ + w_, position_);
        glEnd();
    }
    void StartSliding(const ScreenPosition & cursor) {
        start_position_ = cursor;
        active_ = true;
    }
    void StopSliding() {
        active_ = false;
    }
    void Update(const ScreenPosition & cursor) {

        if (active_) {
            if (isMouseOver(cursor)) {
                position_ = cursor.y;
                UpdateCurrentValue();
            }
        }
    }
    double GetValue() {
        return current_value_;
    }

private:
    void UpdateCurrentValue() {
        current_value_ = kTopValue + (position_ - y_) * (kBottomValue - kTopValue) / h_;
        if (value_callback_ != 0) {
            value_callback_(current_value_);
        }
    }

private:
    int position_;
    double current_value_;
    ScreenPosition start_position_;
    bool active_;
    std::function<void(const double &)> value_callback_;
};

#endif // UI_SLIDER_HPP_
