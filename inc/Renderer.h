#ifndef RENDERER_H
#define RENDERER_H

#include <GLDisplay.h>

class Renderer : public GLDisplay {

    public:
        Renderer();
        void Render();
        void UserKeys( int keycode, bool shift, bool ctrl );
        bool GetQuitCondition();

    private:
        bool m_quitCondition;
};

#endif

