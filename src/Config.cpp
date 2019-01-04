#include <stdlib.h>
#include <Config.h>

Config::Config( std::string filename )
{
    InitVars();
    m_filename = filename;
}

Config::~Config()
{
}

void Config::InitVars()
{
    config_init( &m_config );

    Defaults();
}

void Config::Defaults()
{
    // General
    application_frequency = 20;
    application_timestep = 1.0;
    // Display
    screen_width = 100;
    screen_height = 100;
    world_width = 1.0;
    world_height = 1.0;
    bg_red = 0.0;
    bg_green = 0.0;
    bg_blue = 0.0;
}

void Config::Read()
{
    if( config_read_file( &m_config, m_filename.c_str() ) )
    {
        m_setting_cfg = config_lookup( &m_config, "frame.application" );
        if( m_setting_cfg != NULL )
        {
            config_setting_lookup_int( m_setting_cfg, "frequency", &application_frequency );
            config_setting_lookup_float( m_setting_cfg, "timestep", &application_timestep );
        }

        m_setting_cfg = config_lookup( &m_config, "frame.display" );
        if( m_setting_cfg != NULL )
        {
            config_setting_t *screen_cfg = config_lookup( &m_config, "frame.display.screen" );
            if( screen_cfg != NULL )
            {
                config_setting_lookup_int( screen_cfg, "width", &screen_width );
                config_setting_lookup_int( screen_cfg, "height", &screen_height );
            }

            config_setting_t *world_cfg = config_lookup( &m_config, "frame.display.world" );
            if( world_cfg != NULL )
            {
                config_setting_lookup_float( world_cfg, "width", &world_width );
                config_setting_lookup_float( world_cfg, "height", &world_height );
            }

            config_setting_t *bg_cfg = config_lookup( &m_config, "frame.display.bg" );
            if( bg_cfg != NULL )
            {
                config_setting_lookup_float( bg_cfg, "red", &bg_red );
                config_setting_lookup_float( bg_cfg, "green", &bg_green );
                config_setting_lookup_float( bg_cfg, "blue", &bg_blue );
            }
        }

        config_destroy( &m_config );
    }
    else
    {
        fprintf( stderr, "%s:%d - %s\n", 
            config_error_file(&m_config), 
            config_error_line(&m_config), 
            config_error_text(&m_config) );

        config_destroy( &m_config );

        exit( 1 );
    }
}

