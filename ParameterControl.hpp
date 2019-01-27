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
        slider->SetLabel("OC");
        slider->SetPosition(p);
        slider->SetSize(sw, sh);
        slider->SetInitialValue(universe_->getOctaveCount());
        slider->SetValueCallback(
            std::bind(&Universe::setOctaveCount, universe_, _1)
        );
        sliders_.push_back(slider);

        // frequency
        p.x += xs;
        slider = new UiSlider();
        slider->SetLabel("FQ");
        slider->SetPosition(p);
        slider->SetSize(sw, sh);
        slider->SetInitialValue(universe_->getFrequency());
        slider->SetValueCallback(
            std::bind(&Universe::setFrequency, universe_, _1)
        );
        sliders_.push_back(slider);

        // step size
        p.x += xs;
        slider = new UiSlider();
        slider->SetLabel("SS");
        slider->SetPosition(p);
        slider->SetSize(sw, sh);
        slider->SetInitialValue(universe_->getStepSize());
        slider->SetValueCallback(
            std::bind(&Universe::setStepSize, universe_, _1)
        );
        sliders_.push_back(slider);

        // x, y
        p.x += xs;
        slider = new UiSlider();
        slider->SetLabel("X");
        slider->SetPosition(p);
        slider->SetSize(sw, sh);
        slider->SetInitialValue(universe_->getXPosition());
        slider->SetValueCallback(
            std::bind(&Universe::setXPosition, universe_, _1)
        );
        sliders_.push_back(slider);

        p.x += xs;
        slider = new UiSlider();
        slider->SetLabel("Y");
        slider->SetPosition(p);
        slider->SetSize(sw, sh);
        slider->SetInitialValue(universe_->getYPosition());
        slider->SetValueCallback(
            std::bind(&Universe::setYPosition, universe_, _1)
        );
        sliders_.push_back(slider);

        // min value
        p.x += xs;
        slider = new UiSlider();
        slider->SetLabel("MV");
        slider->SetPosition(p);
        slider->SetSize(sw, sh);
        slider->SetInitialValue(universe_->getMinValue());
        slider->SetValueCallback(
            std::bind(&Universe::setMinValue, universe_, _1)
        );
        sliders_.push_back(slider);

        // z index
        p.x += xs;
        slider = new UiSlider();
        slider->SetLabel("ZI");
        slider->SetPosition(p);
        slider->SetSize(sw, sh);
        slider->SetInitialValue(universe_->getZIndex());
        slider->SetValueCallback(
            std::bind(&Universe::setZIndex, universe_, _1)
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

        glColor4f(0.1, 0.0, 0.0, 0.8);
        glBegin(GL_QUADS);
        glVertex2i(0, 0);
        glVertex2i(width_, 0);
        glVertex2i(width_, height_);
        glVertex2i(0, height_);
        glEnd();

        glColor4f(0.8, 0.5, 0.5, 0.2);
        glBegin(GL_LINE_LOOP);
        glVertex2i(0, 0);
        glVertex2i(width_, 0);
        glVertex2i(width_, height_);
        glVertex2i(0, height_);
        glEnd();

        UniverseParameters params;
        universe_->GetParameters(params);

        const int tx = 20;
        const int ts = 40;
        int ty = 40;

        text_->UseFont(1, 32);
        glColor3f(1.0, 0.7, 0.7);
        glRasterPos2i(tx, ty); ty += ts;
        text_->Print("Octave Count : %d", params.octaveCount);
        glRasterPos2i(tx, ty); ty += ts;
        text_->Print("Frequency    : %.8f", params.frequency);
        glRasterPos2i(tx, ty); ty += ts;
        text_->Print("(X, Y)       : (%.8f, %.8f)", params.x, params.y);
        glRasterPos2i(tx, ty); ty += ts;
        text_->Print("Min Value    : %.8f", params.minValue);
        glRasterPos2i(tx, ty); ty += ts;
        text_->Print("Step Size    : %.8f", params.stepSize);
        glRasterPos2i(tx, ty); ty += ts;
        text_->Print("Z-Index      : %.8f", params.zIndex);
    }
    void InitSlider() {
    }
    void RenderSlider() {

        for (auto s : sliders_) {
            s->Render();
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
