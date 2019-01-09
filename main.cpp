#include "TextRenderer.hpp"
#include "Viewport.hpp"
#include "TestPattern.hpp"

#include <GL/glut.h>

// Variables
// -- cursor position
static int cursor_x_ = 0;
static int cursor_y_ = 0;

// -- mouse button & state
enum {
    btn_NONE = -1,
    btn_LEFT = 0,
    btn_MIDDLE = 1,
    btn_RIGHT = 2,
    btn_SCROLL_UP = 3,
    btn_SCROLL_DOWN = 4,
    state_NONE = -1,
    state_PRESSED = 0,
    state_RELEASED = 1,
};
static int button_ = btn_NONE;
static int state_ = state_NONE;
static char button_names_[5][16] = {
    "Left",
    "Middle",
    "Right",
    "Scroll Up",
    "Scroll Down"
};
static char button_states_[2][16] = {
    "Pressed",
    "Released"
};
// -- window size
static double window_width_ = 1200.0;
static double window_height_ = 1200.0;
static const int MAX_BUFFER = 1024;
// -- world viewport
Viewport vp_;

TextRenderer text_;

void idle() {
    // TODO : Update here
}

void world_enable() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    vp_.Ortho();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void screen_enable() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, window_width_, window_height_, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void render_world() {
    TestPattern::World();
}

void render_ui() {
    TestPattern::Ui(window_width_, window_height_);

    text_.Print(
        50, 100
        , "button(%s), state(%s), x(%d), y(%d)"
        , button_ > -1 ? button_names_[button_] : "n/a"
        , state_ > -1 ? button_states_[state_] : "n/a"
        , cursor_x_
        , cursor_y_
    );
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Render world here
    world_enable();
    render_world();

    // UI (screen space) here.
    screen_enable();
    render_ui();

    glutSwapBuffers();
    glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y) {
    (void)x;
    (void)y;
    switch(key) {
    case 27:
        exit(0); break;
    }
}

void special(int key, int x, int y) {
    // TODO
    (void)key;
    (void)x;
    (void)y;
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
}

void motion(int x, int y) {
    cursor_x_ = x;
    cursor_y_ = y;

    vp_.Pan(cursor_x_, cursor_y_);
    vp_.Zoom(cursor_x_, cursor_y_);
}
void process_left_button() {
    switch (state_) {
    case state_PRESSED:
        break;
    case state_RELEASED:
        break;
    case state_NONE:
    default:
        break;
    }
}
void process_right_button() {
    switch (state_) {
    case state_PRESSED:
        vp_.PanStart(cursor_x_, cursor_y_);
        break;
    case state_RELEASED:
        vp_.PanStop();
        break;
    case state_NONE:
    default:
        break;
    }
}
void process_middle_button() {
    switch (state_) {
    case state_PRESSED:
        vp_.ZoomStart(cursor_x_, cursor_y_);
        break;
    case state_RELEASED:
        vp_.ZoomStop();
        break;
    case state_NONE:
    default:
        break;
    }
}
void mouse(int button, int state, int x, int y) {
    // TODO : Button handling.
    (void)x;
    (void)y;

    button_ = button;
    state_ = state;

    switch(button_) {
    case btn_LEFT:
        process_left_button();
        break;
    case btn_RIGHT:
        process_right_button();
        break;
    case btn_MIDDLE:
        process_middle_button();
        break;
    case btn_SCROLL_UP:
        break;
    case btn_SCROLL_DOWN:
        break;
    case btn_NONE:
    default:
        break;
    }
}

void init() {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(special);
    glutReshapeFunc(reshape);
    glutIdleFunc(idle);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutPassiveMotionFunc(motion);

    glClearColor(0.0, 0.0, 0.0, 0.0);

    vp_.SetWindowSize(window_width_, window_height_);
}

int main(int argc, char **argv) {

    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);

    glutInitWindowSize(window_width_, window_height_);

    glutInitWindowPosition(10, 10);

    glutCreateWindow("Universe Test");

    init();

    glutMainLoop();

    return 0;
}
