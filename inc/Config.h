#ifndef CONFIG_H
#define CONFIG_H

#include <string>
#include <libconfig.h>

class Config 
{
public:
    Config( std::string filename );
    ~Config();
    void Read();

private:
    std::string m_filename;
    config_t m_config;
    config_setting_t *m_setting_cfg;

    void InitVars();
    void Defaults();

public:
    // Config data members
    // Application
    int application_frequency;
    double application_timestep;
    // Display
    int screen_width;
    int screen_height;
    // World
    double world_width;
    double world_height;
    // Background fill color.
    double bg_red;
    double bg_green;
    double bg_blue;
};

#endif

