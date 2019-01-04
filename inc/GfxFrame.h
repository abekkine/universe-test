#ifndef GFXFRAME_H
#define GFXFRAME_H

class Timer;
class Config;
class UniverseRenderer;

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
        UniverseRenderer* m_display;
        bool m_quitCondition;
        unsigned int m_numTicks;
        unsigned int m_ticksPeriod;
        Timer* m_timer;

        // Methods.
        void InitVars();
};

#endif
