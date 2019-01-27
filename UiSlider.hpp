#ifndef UI_SLIDER_HPP_
#define UI_SLIDER_HPP_

#include "UiObject.hpp"
#include "TextRendererFactory.hpp"

class UiSlider : public UiObject {
private:
    const double kTopValue;
    const double kBottomValue;

public:
    UiSlider()
    : kTopValue(1.0)
    , kBottomValue(0.0)
    {
        label_ = "slider";
        position_ = 0;
        UpdateCurrentValue();
        value_callback_ = 0;
        active_ = false;
        text_ = TextRendererFactory::getTextRenderer();
        text_->AddFont(1, "ubuntu_mono.ttf");
    }
    ~UiSlider() {}
    void SetLabel(const std::string & label) {
        label_ = label;
    }
    void SetInitialValue(const double & value) {
        current_value_ = value;
        position_ = slider_position_.y + h_ * (current_value_ - kTopValue) / (kBottomValue - kTopValue);
    }
    void SetPosition(const ScreenPosition & p) {
        UiObject::SetPosition(p);

        label_position_ = p;
        slider_position_ = p;
        slider_position_.y += 50;
        position_ = slider_position_.y;
    }
    void SetValueCallback(std::function<void(const double &)> callback) {
        value_callback_ = callback;
    }
    void Render() {
        int sm = 4;

        glColor4f(0.0, 0.0, 0.0, 0.8);
        glBegin(GL_QUADS);
        glVertex2i(slider_position_.x - sm, slider_position_.y - sm);
        glVertex2i(slider_position_.x + w_ + sm, slider_position_.y - sm);
        glVertex2i(slider_position_.x + w_ + sm, slider_position_.y + h_ + sm);
        glVertex2i(slider_position_.x - sm, slider_position_.y + h_ + sm);
        glEnd();

        glColor3f(1.0, 1.0, 1.0);
        glLineWidth(2.0);
        glBegin(GL_LINE_LOOP);
        glVertex2i(slider_position_.x - sm, slider_position_.y - sm);
        glVertex2i(slider_position_.x + w_ + sm, slider_position_.y - sm);
        glVertex2i(slider_position_.x + w_ + sm, slider_position_.y + h_ + sm);
        glVertex2i(slider_position_.x - sm, slider_position_.y + h_ + sm);
        glEnd();

        glColor3f(1.0, 0.0, 0.0);
        glLineWidth(4.0);
        glBegin(GL_LINES);
        glVertex2i(slider_position_.x, position_);
        glVertex2i(slider_position_.x + w_, position_);
        glEnd();

        glColor3f(1.0, 1.0, 1.0);
        text_->UseFont(1, 32);
        glRasterPos2i(label_position_.x, label_position_.y);
        text_->Print("%s", label_.c_str());
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

    bool isMouseOver(const ScreenPosition & p) {
        return (
            p.y > slider_position_.y &&
            p.y < slider_position_.y + h_ &&
            p.x > slider_position_.x &&
            p.x < slider_position_.x + w_ );
    }
    void UpdateCurrentValue() {
        current_value_ = kTopValue + (position_ - slider_position_.y) * (kBottomValue - kTopValue) / h_;
        if (value_callback_ != 0) {
            value_callback_(current_value_);
        }
    }

private:
    TextRendererInterface * text_;
    std::string label_;
    int position_;
    double current_value_;
    ScreenPosition label_position_;
    ScreenPosition slider_position_;
    ScreenPosition start_position_;
    bool active_;
    std::function<void(const double &)> value_callback_;
};

#endif // UI_SLIDER_HPP_
