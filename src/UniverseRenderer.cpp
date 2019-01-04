#include <UniverseRenderer.h>

UniverseRenderer::UniverseRenderer()
{
    // TODO
    m_quitCondition = false;
}

UniverseRenderer::~UniverseRenderer()
{}

void UniverseRenderer::Render()
{
    // TODO
    glColor3f( 1.0, 1.0, 1.0 );
    glBegin( GL_QUADS );
        glVertex2d( -0.1, -0.1 );
        glVertex2d( -0.1, 0.1 );
        glVertex2d( 0.1, 0.1 );
        glVertex2d( 0.1, -0.1 );
    glEnd();
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
        case SDLK_UP:
        case SDLK_DOWN:
        case SDLK_RIGHT:
        case SDLK_LEFT:
            break;

        case SDLK_SPACE:
            break;

        case SDLK_q:
        case SDLK_ESCAPE:
            m_quitCondition = true;
            break;
    }
}
