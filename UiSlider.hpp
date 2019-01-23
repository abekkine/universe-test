#ifndef UI_SLIDER_HPP_
#define UI_SLIDER_HPP_

#include "UiObject.hpp"

class UiSlider : public UiObject {
public:
    UiSlider() {
        SetValueLimits(0.0, 1.0);
        SetValuePosition(0);
        UpdateCurrentValue();
    }
    ~UiSlider() {}
    void SetValuePosition(const int value) {
        position_ = value;
    }
    void SetValueLimits(const double & bottom, const double & top) {
        bottom_value_ = bottom;
        top_value_ = top;
    }
    void Render() {

    }
    void StartSliding(const ScreenPosition & cursor) {
        slide_start_pos_ = cursor;
        slide_active_ = true;
    }
    void StopSliding() {
        slide_active_ = false;
    }
    void Update(const ScreenPosition & cursor) {
        cursor_ = cursor;
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
};

#endif // UI_SLIDER_HPP_
