#ifndef UI_SLIDER_HPP_
#define UI_SLIDER_HPP_

#include "UiObject.hpp"

class UiSlider : public UiObject {
public:
    UiSlider() {
        SetValueLimits(0.0, 1.0);
        SetValuePosition(0);
        active_ = false;
    }
    ~UiSlider() {}
    void SetValuePosition(const int value) {
        position_ = value;
        UpdateCurrentValue();
    }
    void SetValueLimits(const double & bottom, const double & top) {
        bottom_value_ = bottom;
        top_value_ = top;
        UpdateCurrentValue();
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
        current_value_ = top_value_ + (position_ - y_) * (bottom_value_ - top_value_) / h_;
    }

private:
    int position_;

    double top_value_;
    double bottom_value_;
    double current_value_;

    ScreenPosition start_position_;
    bool active_;
};

#endif // UI_SLIDER_HPP_
