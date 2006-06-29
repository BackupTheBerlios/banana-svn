
#include "main.h"

void process_sdl_events();

void gameloop()
{
    /* printf( "Mouse is at %i,%i\n", get_mouse_x(), get_mouse_y() ); 

    if ( get_active_dialog() != NULL )
        printf( "Dialog open: %s\n", get_active_dialog()->title );
    else
        printf( "Dialog open: None\n" ); */

    if ( get_server_active() )
        server_listen();

    if ( get_client_active() )
        client_listen();

    process_sdl_events();
    draw_scene();
}

void process_sdl_events()
{
    SDL_Event event;
    SDL_Surface *screen=SDL_GetVideoSurface();

    if (!gg_dialog_current())
    {
        //printf( "No dialog mouse bit\n" );
        process_mouse();
    }

    if (!gg_dialog_current())
    {
        //printf( "No dialog keyboard bit\n" );
        process_keyboard();
    }

    while ( SDL_PollEvent( &event ) )
    {
        gg_event_t gg_event;
        
        /* Mouse! */
        if (event.type == SDL_MOUSEMOTION)
        {
           // printf( "Dialog mouse bit\n" );
            gg_dialog_t *dialog = gg_dialog_current();
            if (dialog)
                gg_dialog_mouse_movement(dialog, event.motion.x, event.motion.y);
        }

        /* Keyboard! */
        if (gg_dialog_current())
        {
           // printf( "Dialog keyboard bit\n" );
            gg_event = convert_event(&event);

            if (gg_event.type != GG_EVENT_NONE) 
            {
                if (gg_dialog_current())
                    gg_dialog_input_current(gg_event);
            }
        }

        switch( event.type )
        {    
            case SDL_VIDEORESIZE:
                screen = SDL_SetVideoMode( event.resize.w, event.resize.h, 
                    SCREEN_BPP, get_videoflags() );

                if ( !screen )
				{
				    fprintf( stderr, "Could not get a surface after resize: %s\n", SDL_GetError( ) );
				    quit( 1 );
				}
			    resize_window( event.resize.w, event.resize.h );
			    break;
			case SDL_QUIT:
			    /* handle quit requests */
			    quit(0);
			    break;
			default:
			    break;
		}
	}
}
