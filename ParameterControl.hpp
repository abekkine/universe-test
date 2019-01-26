#ifndef PARAMETER_CONTROL_HPP_
#define PARAMETER_CONTROL_HPP_

#include "TextRendererFactory.hpp"
#include "ScreenPosition.hpp"
#include "UiSlider.hpp"
#include "UniverseParameters.hpp"

#include <iostream>
#include <string>

#include <GL/glut.h>

enum {
    e_OCTAVE = 0,
    e_FREQUENCY,
    e_X,
    e_Y,
    e_MINVALUE,
    e_STEPSIZE,
    e_MAXPARAMS,
};

class ParameterControl {
public:
    ParameterControl() {
        params_ = 0;
        ScreenPosition p;

        // octave count
        p.Set(50, 50);
        slider_[e_OCTAVE].SetPosition(p);
        slider_[e_OCTAVE].SetSize(50, 1500);
        slider_[e_OCTAVE].SetValuePosition(0);
        slider_[e_OCTAVE].SetValueLimits(1, 8);
        // frequency
        p.Set(100, 50);
        slider_[e_FREQUENCY].SetPosition(p);
        slider_[e_FREQUENCY].SetSize(50, 1500);
        slider_[e_FREQUENCY].SetValuePosition(0);
        slider_[e_FREQUENCY].SetValueLimits(0.05, 5.0);
        // x, y
        // min value
        // step size

        text_ = TextRendererFactory::getTextRenderer();
        text_->AddFont(1, "ubuntu_mono.ttf");

        x_ = 100;
        y_ = 100;
        width_ = 200;
        height_ = 50;
    }
    ~ParameterControl() {}
    void AddSliderDouble(double * var_ptr, double min, double max) {
        UiSlider * slider = new UiSlider();
    }
    void RegisterParameters(UniverseParameters * params) {
        params_ = params;
    }
    void SetSize(int w, int h) {
        width_ = w;
        height_ = h;
    }
    void SetPosition(int x, int y) {
        x_ = x;
        y_ = y;
    }
    void StartSlider(const ScreenPosition & cursor) {
        for (int i=0; i<e_MAXPARAMS; ++i) {
            slider_[i].StartSliding(cursor);
        }
    }
    void StopSlider() {
        for (int i=0; i<e_MAXPARAMS; ++i) {
            slider_[i].StopSliding();
        }
    }
    void Update(const ScreenPosition & cursor) {
        cursor_ = cursor;

        for (int i=0; i<e_MAXPARAMS; ++i) {
            slider_[i].Update(cursor_);
        }
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

        for (int i=0; i<e_MAXPARAMS; ++i) {
            slider_[i].Render();

            // glColor3f(1.0, 1.0, 1.0);
            // text_->UseFont(1, 32);
            // glRasterPos2i(200, 100);
            // text_->Print("%.2f", slider_.GetValue());
        }
    }

private:
    TextRendererInterface * text_;
    const int margin_ = 30;
    int x_, y_;
    int width_, height_;
    std::string input_string_;
    ScreenPosition cursor_;

    UiSlider slider_[e_MAXPARAMS];
    UniverseParameters * params_;
};

#endif // PARAMETER_CONTROL_HPP_
