
#include "main.h"

int main()
{
    init_video();
    init_net();

    load_map( "data/test.map" );
    load_tilebay( "data/tiles.png" );
    load_mouse_cursor( "data/mouse_cursor.png" );
    load_fonts();
    init_gui();

    set_camera( 0.0f, 0.0f );

    while(5)
	{
        gameloop();
	}

    quit(0);
    return(0);  
}

void quit( int retcode )
{
    send_disconnect_message();
	SDLNet_Quit();
    SDL_Quit();
    exit( retcode );
}
