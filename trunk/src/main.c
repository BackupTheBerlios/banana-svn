
#include "main.h"

int main( int argc, char *argv[])
{
    init_video();
    init_net();

    load_map( "data/test.map" );
    load_tilebay( "data/tiles.png" );
    load_mouse_cursor( "data/mouse_cursor.png" );
    load_font( "data/big_font.png", "data/big_font.wid" );
    init_gui();

    /* temp! */
    load_player_tex();

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
    if (get_client_active())
        send_disconnect_message();

	SDLNet_Quit();
    SDL_Quit();
    exit( retcode );
}
