#include <stdio.h>
#include <version.h>

#include <Config.h>
#include <GfxFrame.h>

void display_version();

int main( int argc, char *argv[] )
{
    Config *config;
    GfxFrame *frame;

    // Display version information.
    display_version();

    if( argc == 2 )
    {
        config = new Config( argv[1] );
    }
    else
    {
        config = new Config( "../conf/frame.conf" );
    }

    frame = new GfxFrame();

    if (frame->Initialize( config )) {

        frame->Run();
    }

    return 0;
}

void display_version()
{
    printf( "Hg.%s\n", VERSION_STRING );
}
