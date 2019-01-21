#ifndef COMMAND_CONSOLE_HPP_
#define COMMAND_CONSOLE_HPP_

#include "TextRendererFactory.hpp"
#include "ScreenPosition.hpp"

#include <iostream>
#include <string>

#include <GL/glut.h>

class CommandConsole {
private:
    struct {
        int x, y;
        int w, h;
        int pos;

        double top, bottom;
        double value;
    } slider_;
public:
    CommandConsole() {

        slider_.x = 50;
        slider_.y = 50;
        slider_.w = 50;
        slider_.h = 1500;
        slider_.pos = 50;
        slider_.top = 0.0;
        slider_.bottom = 10.0;
        slider_.value = 0.0;

        text_ = TextRendererFactory::getTextRenderer();
        text_->AddFont(1, "ubuntu_mono.ttf");

        x_ = 100;
        y_ = 100;
        width_ = 200;
        height_ = 50;
        slider_active_ = false;
    }
    ~CommandConsole() {}
    void SetSize(int w, int h) {
        width_ = w;
        height_ = h;
    }
    void SetPosition(int x, int y) {
        x_ = x;
        y_ = y;
    }
    void StartSlider(const ScreenPosition & cursor) {
        slider_start_pos_ = cursor;
        slider_active_ = true;
    }
    void StopSlider() {
        slider_active_ = false;
    }
    void Update(const ScreenPosition & cursor) {
        cursor_ = cursor;

        if (slider_active_) {
            if (cursor_.y > slider_.y &&
                cursor_.y < slider_.y + slider_.h) {
                slider_.pos = cursor_.y;

                slider_.value = slider_.top;
                slider_.value += (slider_.pos - slider_.y) * (slider_.bottom - slider_.top) / slider_.h;
            }
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

        int sm = 4;

        glColor3f(1.0, 1.0, 1.0);
        glLineWidth(2.0);
        glBegin(GL_LINE_LOOP);
        glVertex2i(slider_.x - sm, slider_.y - sm);
        glVertex2i(slider_.x + slider_.w + sm, slider_.y - sm);
        glVertex2i(slider_.x + slider_.w + sm, slider_.y + slider_.h + sm);
        glVertex2i(slider_.x - sm, slider_.y + slider_.h + sm);
        glEnd();

        glColor3f(1.0, 0.0, 0.0);
        glLineWidth(4.0);
        glBegin(GL_LINES);
        glVertex2i(slider_.x, slider_.pos);
        glVertex2i(slider_.x + slider_.w, slider_.pos);
        glEnd();

        glColor3f(1.0, 1.0, 1.0);
        text_->UseFont(1, 32);
        glRasterPos2i(200, 100);
        text_->Print("%.2f", slider_.value);
    }

private:
    TextRendererInterface * text_;
    const int margin_ = 30;
    int x_, y_;
    int width_, height_;
    std::string input_string_;
    ScreenPosition cursor_;
    ScreenPosition slider_start_pos_;
    bool slider_active_;
};

#endif // COMMAND_CONSOLE_HPP_
