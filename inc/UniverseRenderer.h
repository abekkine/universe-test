#ifndef UNIVERSE_RENDERER_H
#define UNIVERSE_RENDERER_H

#include <GLDisplay.h>

class Universe;

class UniverseRenderer : public GLDisplay {

    public:
        UniverseRenderer();
        ~UniverseRenderer();
        void Render();
        void UserKeys( int keycode, bool shift, bool ctrl );
        bool GetQuitCondition();
        void SetUniverse(Universe * universe);

    private:
        bool m_quitCondition;
        Universe * m_universe;
        bool m_grid_toggle;
};

#endif // UNIVERSE_RENDERER_H
