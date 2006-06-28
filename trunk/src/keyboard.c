
#include "main.h"

void process_keyboard()
{
    Uint8 *keystate = SDL_GetKeyState(NULL);

    if ( keystate[SDLK_q] )
        quit(0);

    if ( keystate[SDLK_s] )
    { 
        show_server_dialog(FALSE);
    }

    if ( keystate[SDLK_c] )
    { 
        show_client_dialog(FALSE);
    }

    if ( keystate[SDLK_t] )
    { 
        show_chat_dialog(FALSE);
    }

    if ( keystate[SDLK_m] )
    { 
        show_title_dialog(FALSE);
    }

    if ( keystate[SDLK_LEFT] )
        set_camera( get_camera_x()-10, get_camera_y() );

    if ( keystate[SDLK_RIGHT] )
        set_camera( get_camera_x()+10, get_camera_y() );

    if ( keystate[SDLK_UP] )
        set_camera( get_camera_x(), get_camera_y()-10 );

    if ( keystate[SDLK_DOWN] )
        set_camera( get_camera_x(), get_camera_y()+10 );
}
