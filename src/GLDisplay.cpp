#include <GLDisplay.h>

GLDisplay::GLDisplay()
{
    InitVars();
}

GLDisplay::~GLDisplay()
{
}

void GLDisplay::InitVars()
{
    m_zoom_mode = false;
    m_pan_mode = false;
    m_zoom_factor = 0.02;
    m_xOffset = 0.0;
    m_yOffset = 0.0;
    m_zoom_level = 1.0;
    m_screen_width = -1;
    m_screen_height = -1;
    m_background_red = m_background_green = m_background_blue = 0.0;
    m_background_alpha = 1.0;
    m_viewport_left = -1.0;
    m_viewport_right = 1.0;
    m_viewport_bottom = -1.0;
    m_viewport_top = 1.0;
    m_viewport_near = -1.0;
    m_viewport_far = 1.0;
}

void GLDisplay::SetScreenSize( int width, int height )
{
    m_screen_width = width;
    m_screen_height = height;
}

void GLDisplay::SetWorldSize( double width, double height )
{
    m_world_width = width;
    m_world_height = height;
}

bool GLDisplay::Initialize()
{
    bool result = false;
    int rc;

    rc = SDL_Init( SDL_INIT_VIDEO );
    if( rc < 0 )
    {
        fprintf( stderr, "Unable to init SDL: %s\n", SDL_GetError() );
    }
    else
    {
        atexit( SDL_Quit );

        m_screen = SDL_SetVideoMode( m_screen_width, m_screen_height, 16, SDL_OPENGL );
        if( m_screen == NULL )
        {
            fprintf( stderr, "Unable to set %dx%d video: %s\n", m_screen_width, m_screen_height, SDL_GetError() );
        }
        else
        {
            // try to enable text rendering support.
            GLInit();
            Reshape(m_screen->w, m_screen->h);
            result = true;
        }
    }

    return result;
}

void GLDisplay::PreRender()
{
    // Clear screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
}

void GLDisplay::PostRender()
{
    // Swap buffers.
    SDL_GL_SwapBuffers();
}

void GLDisplay::Render()
{
}

void GLDisplay::Update()
{
    PreRender();
    Render();
    PostRender();
}

void GLDisplay::GLInit()
{
    glShadeModel( GL_SMOOTH );
    glClearColor( m_background_red, m_background_green, m_background_blue, m_background_alpha );
    glClearDepth( 1.0f );
    glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );
    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
}

void GLDisplay::UpdateViewport()
{
    double scaled_width;
    double scaled_height;

    scaled_width = 0.5 * m_world_width * m_zoom_level;
    scaled_height = 0.5 * m_world_height * m_zoom_level;

    m_viewport_left = m_xOffset - scaled_width;
    m_viewport_right = m_xOffset + scaled_width;

    m_viewport_top = m_yOffset + scaled_height;
    m_viewport_bottom = m_yOffset - scaled_height;
}

void GLDisplay::Reshape(int width, int height)
{
    if( height == 0 )
    {
        height = 1;
    }

    glViewport( 0, 0, width, height );

    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();

    // Calculate viewport limits.
    UpdateViewport();

    glOrtho( m_viewport_left, m_viewport_right, m_viewport_bottom, m_viewport_top, m_viewport_near, m_viewport_far );

    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
}

bool GLDisplay::PollEvents()
{
    bool result = false;
    SDL_Event event;
    
    while( SDL_PollEvent(&event) )
    {
        switch( event.type )
        {
            case SDL_VIDEORESIZE:
                m_screen = SDL_SetVideoMode(event.resize.w, event.resize.h, 16, SDL_OPENGL|SDL_RESIZABLE);
                if( m_screen )
                {
                    Reshape(m_screen->w, m_screen->h);
                }
                else
                {
                    puts( "Unable to set video mode on reshape." );
                }
                break;

            case SDL_KEYDOWN:
                ProcessKeys( event.key );
                break;

            case SDL_MOUSEMOTION:
                MouseMotion( event.motion );
                break;

            case SDL_MOUSEBUTTONDOWN:
            case SDL_MOUSEBUTTONUP:
                MouseButton( event.button );
                break;

            case SDL_QUIT:
                result = true;
                break;
        }
    }

    return result;
}

void GLDisplay::UserKeys( int keycode, bool shift, bool ctrl )
{
    keycode = keycode;
    shift = shift;
    ctrl = ctrl;
}

void GLDisplay::MouseMotion( SDL_MouseMotionEvent& motion )
{
    int dx, dy;
    double scale_x, scale_y;

    m_mouse_x = motion.x;
    m_mouse_y = motion.y;

    if( m_zoom_mode )
    {
        dy = m_mouse_y - m_zoom_start_pos;

        m_zoom_level += dy * m_zoom_factor;

        m_zoom_start_pos = m_mouse_y;

        Reshape( m_screen->w, m_screen->h );
    }

    if( m_pan_mode )
    {
        scale_x = (m_viewport_right - m_viewport_left) / (m_world_width * m_screen_width);
        scale_y = (m_viewport_bottom - m_viewport_top) / (m_world_height * m_screen_height);

        dx = m_mouse_x - m_pan_start_x;
        dy = m_mouse_y - m_pan_start_y;

        m_xOffset -= dx * scale_x;
        m_yOffset -= dy * scale_y;

        m_pan_start_x = m_mouse_x;
        m_pan_start_y = m_mouse_y;

        Reshape( m_screen->w, m_screen->h );
    }
}

void GLDisplay::MouseButton( SDL_MouseButtonEvent& button )
{
    switch( button.button )
    {
        case SDL_BUTTON_LEFT:
            // No action taken.
            break;

        case SDL_BUTTON_RIGHT:
            if( button.state == SDL_PRESSED )
            {
                m_pan_mode = true;
                m_pan_start_x = m_mouse_x;
                m_pan_start_y = m_mouse_y;
            }
            else
            {
                m_pan_mode = false;
            }
            break;

        case SDL_BUTTON_MIDDLE:
            if( button.state == SDL_PRESSED )
            {
                m_zoom_mode = true;
                m_zoom_start_pos = m_mouse_y;
            }
            else
            {
                m_zoom_mode = false;
            }
            break;

        case SDL_BUTTON_WHEELUP:
            m_zoom_level += 0.01;
            Reshape(m_screen->w, m_screen->h);
            break;

        case SDL_BUTTON_WHEELDOWN:
            m_zoom_level -= 0.01;
            Reshape(m_screen->w, m_screen->h);
            break;
    }
}

void GLDisplay::ProcessKeys( SDL_KeyboardEvent& key )
{
    int key_code;
    bool shiftState;
    bool ctrlState;
    SDLMod modifier = SDL_GetModState(); 
    key_code = (int) key.keysym.sym;

    shiftState = ( (modifier & (KMOD_LSHIFT|KMOD_RSHIFT)) != 0);
    ctrlState  = ( (modifier & (KMOD_LCTRL|KMOD_RCTRL)) != 0);

    UserKeys( key_code, shiftState, ctrlState );
}

void GLDisplay::SetBGColor( float r, float g, float b )
{
    m_background_red = r;
    m_background_green = g;
    m_background_blue = b;
}

void GLDisplay::SetBGAlpha( float alpha )
{
    m_background_alpha = alpha;
}
