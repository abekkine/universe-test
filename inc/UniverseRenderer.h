#ifndef UNIVERSE_RENDERER_H
#define UNIVERSE_RENDERER_H

#include <GLDisplay.h>

class UniverseRenderer : public GLDisplay {

    public:
        UniverseRenderer();
        ~UniverseRenderer();
        void Render();
        void UserKeys( int keycode, bool shift, bool ctrl );
        bool GetQuitCondition();

    private:
        bool m_quitCondition;
};

#endif // UNIVERSE_RENDERER_H
