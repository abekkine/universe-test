#include <GfxFrame.h>

GfxFrame::GfxFrame()
{
    InitVars();
}

GfxFrame::~GfxFrame()
{
}

void GfxFrame::Initialize( Config *config )
{
    if( config == NULL )
    {
        return;
    }

    m_config = config;
    m_timer = new Timer();
    m_display = new Renderer();

    // Read configuration file.
    m_config->Read();

    // Configure display.
    m_display->SetScreenSize( m_config->screen_width, m_config->screen_height );
    m_display->SetWorldSize( m_config->world_width, m_config->world_height );
    m_display->SetBGColor( m_config->bg_red, m_config->bg_green, m_config->bg_blue );
    m_display->Initialize();

    // Configure application timer
    if( m_config->application_frequency > 0 )
    {
        m_ticksPeriod = 1000000 / m_config->application_frequency;
    }
    else
    {
        m_ticksPeriod = 0;
    }
    m_timer->Reset();

    m_timeStep = m_config->application_timestep;

    // TODO
}

void GfxFrame::Run()
{
    while( m_quitCondition == false )
    {
        m_numTicks = m_timer->GetElapsed();
        if( m_numTicks > m_ticksPeriod )
        {
            Update( m_timeStep );
            m_timer->Reset();
        }

        m_display->Update();
        m_display->PollEvents();

        m_quitCondition = m_display->GetQuitCondition();
    }
}

void GfxFrame::Update( double timestep )
{
    // TODO
    timestep = timestep;
}

void GfxFrame::InitVars()
{
    m_config = 0;
    m_display = 0;
    m_timer = 0;

    m_quitCondition = false;
    m_numTicks = 0;
    m_ticksPeriod = 0;
}
