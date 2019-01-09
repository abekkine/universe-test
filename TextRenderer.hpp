#ifndef TEXT_RENDERER_HPP_
#define TEXT_RENDERER_HPP_

#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#include <GL/glut.h>

class TextRenderer {
private:
    enum {
        MAX_BUFFER_SIZE = 1024,
    };
public:
    TextRenderer() {
        font_ = GLUT_BITMAP_9_BY_15;
    }
    ~TextRenderer() {}
    void Print(int x, int y, const char * format, ...) {
        memset(print_buffer_, 0, MAX_BUFFER_SIZE);

        va_list args;
        va_start(args, format);
        vsnprintf(print_buffer_, MAX_BUFFER_SIZE, format, args);
        va_end(args);

        glRasterPos2i(x, y);
        for (unsigned int i=0; i<strlen(print_buffer_); ++i) {
            glutBitmapCharacter(font_, print_buffer_[i]);
        }
    }

private:
    char print_buffer_[MAX_BUFFER_SIZE];
    void * font_;
};

#endif // TEXT_RENDERER_HPP_
