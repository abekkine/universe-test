#include <UniverseRenderer.h>
#include <Universe.h>

#include <vector>
#include <iostream>

UniverseRenderer::UniverseRenderer()
{
    m_quitCondition = false;
    m_universe = 0;
    m_grid_toggle = true;
}

UniverseRenderer::~UniverseRenderer()
{}

void UniverseRenderer::SetUniverse(Universe * universe) {
    m_universe = universe;
}

void UniverseRenderer::Render()
{
    if (m_universe != 0) {
        std::vector<Universe::StarInfo> stars;
        m_universe->GetStars(m_xOffset, m_yOffset, stars);
        if (! stars.empty()) {
            // glLineWidth(1.0);
            // glColor4f(1.0, 1.0, 1.0, 0.2);
            // glBegin(GL_LINES);
            // for (auto p : stars) {
            //     glVertex2d(p.x, p.y);
            //     glVertex2d(p.x + p.dx, p.y + p.dy);
            // }
            // glEnd();
            glPointSize(8.0);
            glBegin(GL_POINTS);
            for (auto p : stars) {
                if (m_grid_toggle) {
                    glColor4f(1.0, 0.0, 0.0, 0.2);
                    glVertex2d(p.x, p.y);
                }
                // float c = p.value;
                // glColor3f(c, c, c);
                // glColor3f(1.0, 1.0, 1.0);
                glColor3dv(p.color_ptr);
                glVertex2d(p.x + p.dx, p.y + p.dy);
            }
            glEnd();
        }
    }
}

bool UniverseRenderer::GetQuitCondition()
{
    return m_quitCondition;
}

void UniverseRenderer::UserKeys( int keycode, bool shift, bool ctrl )
{
    // In order to avoid warning messages when modifier keys are not being used.
    ctrl = ctrl;
    shift = shift;

    switch( keycode )
    {
        case SDLK_SPACE:
            m_grid_toggle = ! m_grid_toggle;
            break;
        case SDLK_w: // x_position +
            m_universe->ParamAddXPosition(1.0); break;
        case SDLK_s: // x_position -
            m_universe->ParamAddXPosition(-1.0); break;
        case SDLK_e: // y_position +
            m_universe->ParamAddYPosition(1.0); break;
        case SDLK_d: // y_position -
            m_universe->ParamAddYPosition(-1.0); break;
        case SDLK_r: // min_value +
            m_universe->ParamAddMinValue(0.01); break;
        case SDLK_f: // min_value -
            m_universe->ParamAddMinValue(-0.01); break;
        case SDLK_t: // step_size +
            m_universe->ParamAddStepSize(0.1); break;
        case SDLK_g: // step_size -
            m_universe->ParamAddStepSize(-0.1); break;
        case SDLK_y: // frequency +
            m_universe->ParamAddFrequency(0.1); break;
        case SDLK_h: // frequency -
            m_universe->ParamAddFrequency(-0.1); break;
            break;
        case SDLK_u: // octave_count +
            m_universe->ParamAddOctaveCount(1); break;
        case SDLK_j: // octave_count -
            m_universe->ParamAddOctaveCount(-1); break;
        case SDLK_UP:
        case SDLK_DOWN:
        case SDLK_RIGHT:
        case SDLK_LEFT:
            break;

        case SDLK_q:
        case SDLK_ESCAPE:
            m_quitCondition = true;
            break;
    }
}
