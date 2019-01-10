#include "TextRenderer.hpp"
#include "Viewport.hpp"
#include "TestPattern.hpp"
#include "ButtonProcessor.hpp"

#include <GL/glut.h>

// Variables
// -- cursor position
static int cursor_x_ = 0;
static int cursor_y_ = 0;

// -- mouse button & state
ButtonProcessor right_mouse_processor_(btn_RIGHT);
ButtonProcessor middle_mouse_processor_(btn_MIDDLE);

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

void right_mouse_down() {
    vp_.PanStart(cursor_x_, cursor_y_);
}

void right_mouse_up() {
    vp_.PanStop();
}

void middle_mouse_down() {
    vp_.ZoomStart(cursor_x_, cursor_y_);
}

void middle_mouse_up() {
    vp_.ZoomStop();
}

void mouse(int button, int state, int x, int y) {
    // Button handling.
    (void)x;
    (void)y;

    switch(button) {
    case btn_RIGHT:
        right_mouse_processor_.Process(static_cast<StateType>(state));
        break;
    case btn_MIDDLE:
        middle_mouse_processor_.Process(static_cast<StateType>(state));
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

    right_mouse_processor_.RegisterHandlers(
        right_mouse_down,
        right_mouse_up
    );

    middle_mouse_processor_.RegisterHandlers(
        middle_mouse_down,
        middle_mouse_up
    );
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
