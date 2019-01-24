#ifndef PARAMETER_CONTROL_HPP_
#define PARAMETER_CONTROL_HPP_

#include "TextRendererFactory.hpp"
#include "ScreenPosition.hpp"
#include "UiSlider.hpp"

#include <iostream>
#include <string>

#include <GL/glut.h>

class ParameterControl {
public:
    ParameterControl() {

        ScreenPosition p(50, 50);
        slider_.SetPosition(p);
        slider_.SetSize(50, 1500);
        slider_.SetValuePosition(50);
        slider_.SetValueLimits(10.0, 0.0);

        text_ = TextRendererFactory::getTextRenderer();
        text_->AddFont(1, "ubuntu_mono.ttf");

        x_ = 100;
        y_ = 100;
        width_ = 200;
        height_ = 50;
    }
    ~ParameterControl() {}
    void SetSize(int w, int h) {
        width_ = w;
        height_ = h;
    }
    void SetPosition(int x, int y) {
        x_ = x;
        y_ = y;
    }
    void StartSlider(const ScreenPosition & cursor) {
        slider_.StartSliding(cursor);
    }
    void StopSlider() {
        slider_.StopSliding();
    }
    void Update(const ScreenPosition & cursor) {
        cursor_ = cursor;

        slider_.Update(cursor_);
    }
    void InputSpecialChar(int c) {
        // TODO
    }
    void InputRegularChar(char c) {
        input_string_ += c;
    }
    void Render() {

        glPushMatrix();
        glLoadIdentity();
        glTranslated(x_, y_, 0.0);

        RenderFrame();

        glPopMatrix();

        RenderSlider();
    }

private:
    void RenderFrame() {

        glColor4f(0.0, 0.0, 1.0, 0.3);
        glBegin(GL_QUADS);
        glVertex2i(0, 0);
        glVertex2i(width_, 0);
        glVertex2i(width_, height_);
        glVertex2i(0, height_);
        glEnd();
    }
    void InitSlider() {
    }
    void RenderSlider() {

        slider_.Render();

        glColor3f(1.0, 1.0, 1.0);
        text_->UseFont(1, 32);
        glRasterPos2i(200, 100);
        text_->Print("%.2f", slider_.GetValue());
    }

private:
    TextRendererInterface * text_;
    const int margin_ = 30;
    int x_, y_;
    int width_, height_;
    std::string input_string_;
    ScreenPosition cursor_;

    UiSlider slider_;
};

#endif // PARAMETER_CONTROL_HPP_
