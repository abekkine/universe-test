#ifndef GLDISPLAY_H
#define GLDISPLAY_H

#include <SDL/SDL.h>
#include <GL/gl.h>

class GLDisplay {

    public:
        GLDisplay();
        virtual ~GLDisplay();

        void SetScreenSize( int width, int height );
        void SetWorldSize( double width, double height );
        bool Initialize();
        void PreRender();
        void PostRender();
        virtual void Render()=0;
        virtual void UserKeys( int keycode, bool shift, bool ctrl );
        void Update();
        void Reshape(int w, int h);
        bool PollEvents();
        void SetBGColor( float r, float g, float b );
        void SetBGAlpha( float alpha );

    protected:
        double m_xOffset;
        double m_yOffset;

    private:
        SDL_Surface *m_screen;
        int m_screen_width;
        int m_screen_height;
        double m_world_width;
        double m_world_height;
        double m_zoom_level;
        double m_zoom_start_pos;
        double m_pan_start_x;
        double m_pan_start_y;

        int m_mouse_x;
        int m_mouse_y;
        bool m_zoom_mode;
        bool m_pan_mode;
        double m_zoom_factor;


        float m_background_red;
        float m_background_green;
        float m_background_blue;
        float m_background_alpha;

        double m_viewport_left;
        double m_viewport_right;
        double m_viewport_bottom;
        double m_viewport_top;
        double m_viewport_near;
        double m_viewport_far;

        // Methods.
        void InitVars();
        void GLInit();
        void UpdateViewport();
        void ProcessKeys( SDL_KeyboardEvent& key );
        void MouseMotion( SDL_MouseMotionEvent& motion );
        void MouseButton( SDL_MouseButtonEvent& button );
};


#endif
