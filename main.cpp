#include "Viewport.hpp"
#include "TestPattern.hpp"
#include "ButtonProcessor.hpp"
#include "ScreenPosition.hpp"
#include "ParameterControl.hpp"
#include "TextRendererFactory.hpp"
#include "Universe.h"

#include <GL/glut.h>
#include <math.h>

// Variables
// -- stars from universe
std::vector<Universe::StarInfo> stars_;

// -- cursor position
ScreenPosition cursor_;

// -- selection
Universe::StarInfo selected_star_;
bool selected_;

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

// -- parameter value controller
ParameterControl control_;

// -- text
TextRendererInterface * text_ = 0;

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

void render_selection() {

    if (!selected_) {
        return;
    }

    glPointSize(16.0);
    glColor4f(1.0, 1.0, 0.7, 0.6);
    glBegin(GL_POINTS);
    glVertex2d(selected_star_.x, selected_star_.y);
    glEnd();
}

void render_world() {
    // TestPattern::World();
    WorldPosition center;
    vp_.GetCenter(center);
    double size = vp_.GetSize();
    universe_.GetStars(center.x, center.y, size, stars_);
    if (! stars_.empty()) {

        render_selection();

        for (auto p : stars_) {
            glPointSize(8.0 * p.size);
            glBegin(GL_POINTS);
            glColor3fv(p.color_ptr);
            // glColor3d(
            //     p.color_ptr[0] + p.color_dev,
            //     p.color_ptr[1] + p.color_dev,
            //     p.color_ptr[2] + p.color_dev
            // );
            glVertex2d(p.x, p.y);
            glEnd();
        }
    }
}

void render_position_info() {
    const int pw = 700;
    const int ph = 300;
    const int px = window_width_ - pw - 20;
    const int py = window_height_ - ph - 20;
    const int mg = 10;

    text_->UseFont(2, 40);

    glPushMatrix();
    glLoadIdentity();

    glColor4f(0.5, 1.0, 0.5, 0.5);
    glLineWidth(1.0);
    glBegin(GL_LINE_LOOP);
    glVertex2i(px, py);
    glVertex2i(px + pw, py);
    glVertex2i(px + pw, py + ph);
    glVertex2i(px, py + ph);
    glEnd();

    glColor3d(0.5, 1.0, 0.5);
    int ty = py + 5*mg;
    const int tx = px + 2*mg;
    const int ts = 50;

    WorldPosition w_screen_center;
    WorldPosition w_cursor_position;
    vp_.GetCenter(w_screen_center);
    vp_.GetWorldForCursor(cursor_, w_cursor_position);
    glRasterPos2i(tx, ty); ty += ts;
    text_->Print("Center @ (%.5f, %.5f)", w_screen_center.x, w_screen_center.y);
    glRasterPos2i(tx, ty); ty += ts;
    text_->Print("Screen size @ %.5f", vp_.GetSize());
    glRasterPos2i(tx, ty); ty += ts;
    text_->Print("Cursor @ (%4d, %4d)", cursor_.x, cursor_.y);
    glRasterPos2i(tx, ty); ty += ts;
    text_->Print("World @ (%.4f, %.4f)", w_cursor_position.x, w_cursor_position.y);

    glPopMatrix();
}

void render_star_info() {

    const int pw = 700;
    const int ph = 300;
    const int px = window_width_ - 2 * (pw + 20);
    const int py = window_height_ - ph - 20;
    const int mg = 10;

    text_->UseFont(2, 40);

    glPushMatrix();
    glLoadIdentity();

    glColor4f(0.5, 0.5, 1.0, 0.5);
    glLineWidth(1.0);
    glBegin(GL_LINE_LOOP);
    glVertex2i(px, py);
    glVertex2i(px + pw, py);
    glVertex2i(px + pw, py + ph);
    glVertex2i(px, py + ph);
    glEnd();

    glColor3d(0.5, 0.5, 1.0);
    int ty = py + 5*mg;
    const int tx = px + 2*mg;
    const int ts = 50;

    glRasterPos2i(tx, ty); ty += ts;
    text_->Print("Star Info");
    if (selected_) {
        glRasterPos2i(tx, ty); ty += ts;
        text_->Print("Pos(%.4f, %.4f)", selected_star_.x, selected_star_.y);
        glRasterPos2i(tx, ty); ty += ts;
        text_->Print("Val(%.6f)", selected_star_.value);
        glRasterPos2i(tx, ty); ty += ts;
        text_->Print("Size(%.2f)", selected_star_.size);
    }
    glPopMatrix();
}

void render_ui() {
    TestPattern::Ui(window_width_, window_height_);

    control_.Render();

    render_position_info();

    render_star_info();
}

double distance_square(const WorldPosition & p1, const WorldPosition & p2) {
    const double dx = p1.x - p2.x;
    const double dy = p1.y - p2.y;

    return sqrt(dx*dx + dy*dy);
}

void update_selection() {

    WorldPosition w_cursor_position;
    vp_.GetWorldForCursor(cursor_, w_cursor_position);

    selected_ = false;
    for (auto s : stars_) {

        const double distance = distance_square(w_cursor_position, WorldPosition(s.x, s.y));
        if (distance < (5.0 * vp_.GetPixelSize())) {
            selected_star_ = s;
            selected_ = true;
            break;
        }
    }
}

void init_application() {

    text_ = TextRendererFactory::getTextRenderer();
    text_->AddFont(2, "ubuntu_mono.ttf");

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

    control_.SetPosition(46, window_height_ - 400);
    control_.SetSize(478, 350);
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

        update_selection();

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
