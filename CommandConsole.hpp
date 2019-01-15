#ifndef COMMAND_CONSOLE_HPP_
#define COMMAND_CONSOLE_HPP_

#include "TextRenderer.hpp"

#include <string>

#include <GL/glut.h>

class CommandConsole {
public:
    CommandConsole() {
        x_ = 100;
        y_ = 100;
        width_ = 200;
        height_ = 50;
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
        RenderInput();
        RenderCursor();

        glPopMatrix();
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
    void RenderInput() {

        glColor3f(1.0, 1.0, 1.0);
        text_.Print(margin_, margin_, input_string_.c_str()); 
    }
    void RenderCursor() {
        static int blink_counter = 0;
        ++blink_counter;
        if (blink_counter < 20) {

            int cursor_pos = input_string_.length() * 9;
            text_.Print(margin_ + cursor_pos, margin_, "_");
        }
        else if (blink_counter < 40) {
        }
        else {
            blink_counter = 0;
        }
    }

private:
    TextRenderer text_;
    const int margin_ = 20;
    int x_, y_;
    int width_, height_;
    std::string input_string_;
};

#endif // COMMAND_CONSOLE_HPP_

