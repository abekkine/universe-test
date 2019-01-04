#ifndef GFXFRAME_H
#define GFXFRAME_H

#include <Timer.h>
#include <Config.h>
#include <Renderer.h>

class GfxFrame {
    public:
        GfxFrame();
        ~GfxFrame();

        bool Initialize( Config *config );
        void Run();
        void Update( double timestep );

    private:
        // Variables.
        double m_timeStep;
        Config* m_config;
        Renderer* m_display;
        bool m_quitCondition;
        unsigned int m_numTicks;
        unsigned int m_ticksPeriod;
        Timer* m_timer;

        // Methods.
        void InitVars();
};

#endif
