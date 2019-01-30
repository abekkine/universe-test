#include "Viewport.hpp"
#include "TestPattern.hpp"
#include "ButtonProcessor.hpp"
#include "ScreenPosition.hpp"
#include "ParameterControl.hpp"
#include "Universe.h"

#include <GL/glut.h>

// Variables
// -- cursor position
ScreenPosition cursor_;

// -- mouse button & state
ButtonProcessor left_mouse_processor_;
ButtonProcessor right_mouse_processor_;
ButtonProcessor middle_mouse_processor_;
ButtonProcessor scroll_wheel_processor_;

// -- window size
static double window_width_ = 2048.0;
static double window_height_ = 2048.0;
static const int MAX_BUFFER = 1024;

// -- world viewport
Viewport vp_;

// -- universe
Universe universe_;

// -- command console
ParameterControl control_;

void left_mouse_down() {
    control_.StartSlider(cursor_);
}

void left_mouse_up() {
    control_.StopSlider();
}

void right_mouse_down() {
    vp_.PanStart(cursor_);
}

void right_mouse_up() {
    vp_.PanStop();
}

void middle_mouse_down() {
    vp_.ZoomStart(cursor_);
}

void middle_mouse_up() {
    vp_.ZoomStop();
}

static int wheel_dir_ = -1;
void wheel_up() {
    switch (wheel_dir_) {
    case btn_SCROLL_UP:
        vp_.ZoomInAt(cursor_);
        break;
    case btn_SCROLL_DOWN:
        vp_.ZoomOutAt(cursor_);
        break;
    default:
        break;
    }
}

std::vector<Universe::StarInfo> stars_;
void render_world() {
//    TestPattern::World();
    WorldPosition center;
    vp_.GetCenter(center);
    double size = vp_.GetSize();
    universe_.GetStars(center.x, center.y, size, stars_);
    if (! stars_.empty()) {
        for (auto p : stars_) {
            glPointSize(8.0 * p.size);
            glBegin(GL_POINTS);
            glColor3d(
                p.color_ptr[0] + p.color_dev,
                p.color_ptr[1] + p.color_dev,
                p.color_ptr[2] + p.color_dev
            );
            glVertex2d(p.x + p.dx, p.y + p.dy);
            glEnd();
        }
    }
}

void render_ui() {
    TestPattern::Ui(window_width_, window_height_);

    control_.Render();
}

void init_application() {
    vp_.SetWindowSize(window_width_, window_height_);

    left_mouse_processor_.RegisterHandlers(
        left_mouse_down,
        left_mouse_up
    );

    right_mouse_processor_.RegisterHandlers(
        right_mouse_down,
        right_mouse_up
    );

    middle_mouse_processor_.RegisterHandlers(
        middle_mouse_down,
        middle_mouse_up
    );

    scroll_wheel_processor_.RegisterHandlers(
        0,
        wheel_up
    );

    control_.SetPosition(50, window_height_ - 400);
    control_.SetSize(window_width_ - 100, 350);
    control_.SetUniverse(&universe_);
    control_.SetViewport(&vp_);
    control_.Init();
}

namespace display {

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
        switch(key) {
        case 27:
            exit(0); break;
        default:
            control_.InputRegularChar(key);
            break;
        }
    }

    void special(int key, int x, int y) {

        control_.InputSpecialChar(key);
    }

    void reshape(int w, int h) {
        glViewport(0, 0, w, h);
    }

    void motion(int x, int y) {
        cursor_.Set(x, y);

        control_.Update(cursor_);

        vp_.UpdateCursor(cursor_);
    }

    void mouse(int button, int state, int x, int y) {
        // Button handling.
        switch(button) {
        case btn_LEFT:
            left_mouse_processor_.Process(static_cast<StateType>(state));
            break;
        case btn_RIGHT:
            right_mouse_processor_.Process(static_cast<StateType>(state));
            break;
        case btn_MIDDLE:
            middle_mouse_processor_.Process(static_cast<StateType>(state));
            break;
        case btn_SCROLL_UP:
        case btn_SCROLL_DOWN:
            wheel_dir_ = button;
            scroll_wheel_processor_.Process(static_cast<StateType>(state));
            break;
        case btn_NONE:
        default:
            break;
        }
    }

    void init(int argc, char **argv) {

        glutInit(&argc, argv);
        glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_MULTISAMPLE);
        glutInitWindowSize(window_width_, window_height_);
        glutInitWindowPosition(10, 10);
        glutCreateWindow("Universe Test");

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glEnable(GL_MULTISAMPLE);

        glutDisplayFunc(display);
        glutKeyboardFunc(keyboard);
        glutSpecialFunc(special);
        glutReshapeFunc(reshape);
        glutMouseFunc(mouse);
        glutMotionFunc(motion);
        glutPassiveMotionFunc(motion);

        glClearColor(0.0, 0.0, 0.0, 0.0);
    }

    void run() {
        glutMainLoop();
    }

} // namespace display

int main(int argc, char **argv) {

    display::init(argc, argv);

    init_application();

    display::run();

    return 0;
}
