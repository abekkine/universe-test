#ifndef PARAMETER_CONTROL_HPP_
#define PARAMETER_CONTROL_HPP_

#include "TextRendererFactory.hpp"
#include "ScreenPosition.hpp"
#include "UiSlider.hpp"
#include "UniverseParameters.hpp"
#include "Universe.h"

#include <iostream>
#include <string>
#include <vector>
#include <functional>

#include <GL/glut.h>

class ParameterControl {
public:
    ParameterControl() {
        x_ = 100;
        y_ = 100;
        width_ = 200;
        height_ = 50;

        text_ = TextRendererFactory::getTextRenderer();
        text_->AddFont(1, "ubuntu_mono.ttf");
    }
    ~ParameterControl() {}

    void Init() {
        ScreenPosition p(50, 50);
        const int sw = 50;
        const int sh = 1500;
        const int xs = 70;

        UiSlider * slider = 0;

        // Shortcut for placeholders
        using namespace std::placeholders;

        // octave count
        slider = new UiSlider();
        slider->SetPosition(p);
        slider->SetSize(sw, sh);
        slider->SetValueCallback(
            std::bind(&Universe::setOctaveCount, universe_, _1)
        );
        sliders_.push_back(slider);

        // frequency
        p.x += xs;
        slider = new UiSlider();
        slider->SetPosition(p);
        slider->SetSize(sw, sh);
        slider->SetValueCallback(
            std::bind(&Universe::setFrequency, universe_, _1)
        );
        sliders_.push_back(slider);

        // step size
        p.x += xs;
        slider = new UiSlider();
        slider->SetPosition(p);
        slider->SetSize(sw, sh);
        slider->SetValueCallback(
            std::bind(&Universe::setStepSize, universe_, _1)
        );
        sliders_.push_back(slider);

        // x, y
        p.x += xs;
        slider = new UiSlider();
        slider->SetPosition(p);
        slider->SetSize(sw, sh);
        slider->SetValueCallback(
            std::bind(&Universe::setXPosition, universe_, _1)
        );
        sliders_.push_back(slider);

        p.x += xs;
        slider = new UiSlider();
        slider->SetPosition(p);
        slider->SetSize(sw, sh);
        slider->SetValueCallback(
            std::bind(&Universe::setYPosition, universe_, _1)
        );
        sliders_.push_back(slider);

        // min value
        p.x += xs;
        slider = new UiSlider();
        slider->SetPosition(p);
        slider->SetSize(sw, sh);
        slider->SetValueCallback(
            std::bind(&Universe::setMinValue, universe_, _1)
        );
        sliders_.push_back(slider);
    }

    void SetUniverse(Universe * universe) {
        universe_ = universe;
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
        for (auto s : sliders_) {
            s->StartSliding(cursor);
        }
    }
    void StopSlider() {
        for (auto s : sliders_) {
            s->StopSliding();
        }
    }

    void Update(const ScreenPosition & cursor) {
        cursor_ = cursor;

        for (auto s : sliders_) {
            s->Update(cursor_);
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

        for (auto s : sliders_) {
            s->Render();

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
    Universe * universe_;
    std::vector<UiSlider *> sliders_;
};

#endif // PARAMETER_CONTROL_HPP_
