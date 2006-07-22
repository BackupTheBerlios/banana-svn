
#include "main.h"

void process_sdl_events();

void gameloop()
{
    if ( get_server_active() )
        server_listen();

    if ( get_client_active() )
        client_listen();

    process_sdl_events();
    render_scene();
}

void process_sdl_events()
{
    SDL_Event event;
    Uint8 *keystate;
    SDL_Surface *screen=SDL_GetVideoSurface();
    int player_moved=FALSE;

    SDL_PumpEvents(); 

    keystate = SDL_GetKeyState(NULL);

    /* Editing */
    if (get_editing())
    {
        /* Camera controls */
        if ( keystate[SDLK_LEFT] )
           set_camera( get_camera_x()-10, get_camera_y() );
        if ( keystate[SDLK_RIGHT] )
           set_camera( get_camera_x()+10, get_camera_y() );
        if ( keystate[SDLK_UP] )
           set_camera( get_camera_x(), get_camera_y()-10 );
        if ( keystate[SDLK_DOWN] )
           set_camera( get_camera_x(), get_camera_y()+10 );

        /* Mouse */
        if( SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(1) )
        {        
            if ( get_tile(get_player_layer(), (get_camera_x()+get_mouse_x())/32, (get_camera_y()+get_mouse_y())/32 ) !=1 )
            {
                set_tile(get_player_layer(), (get_camera_x()+get_mouse_x())/32, (get_camera_y()+get_mouse_y())/32, 1);
    
                /* TEMporary!!!*/
                net_change_tile( (get_camera_x()+get_mouse_x())/32, (get_camera_y()+get_mouse_y())/32 );
                /* ----^ Temporary!! */
            }
        }
    }

    /* temporary.. move player.. */
    if ( keystate[SDLK_LEFT] )
    {
        move_player( get_local_player_index(), -5.0f * PLAYER_MOVE_SPEED, 0.0f );
        player_moved=TRUE;
    }
    else if ( keystate[SDLK_RIGHT] )
    {
        move_player( get_local_player_index(), 5.0f * PLAYER_MOVE_SPEED, 0.0f );
        player_moved=TRUE;
    }
    if ( keystate[SDLK_UP] )
    {
        move_player( get_local_player_index(), 0.0f, -5.0f * PLAYER_MOVE_SPEED );
        player_moved=TRUE;
    }
    else if ( keystate[SDLK_DOWN] )
    {
        move_player( get_local_player_index(), 0.0f, 5.0f * PLAYER_MOVE_SPEED );
        player_moved=TRUE;
    }

    if ( player_moved )
    {   
        send_playermoved_message( get_local_player_index(), 
            (int)get_player(get_local_player_index())->xpos, 
            (int)get_player(get_local_player_index())->ypos );
    }

    /* Show player list? */
    if ( (get_client_active() || get_server_active()) && keystate[SDLK_TAB] )
        set_show_player_list(TRUE);
    else
        set_show_player_list(FALSE);     

    while ( SDL_PollEvent( &event ) )
    {
        gg_event_t gg_event;
        
        /* GameGUI active? */
        if ( gg_dialog_current() )
        {
            /* Mouse */
            if (event.type == SDL_MOUSEMOTION)
            {              
                gg_dialog_t *dialog = gg_dialog_current();               
                gg_dialog_mouse_movement(dialog, event.motion.x, event.motion.y);
            }

            /* Keyboard! */
            if (gg_dialog_current())
            {
                gg_event = convert_event(&event);

                if (gg_event.type != GG_EVENT_NONE) 
                {
                    if (gg_dialog_current())
                        gg_dialog_input_current(gg_event);
                }
            }
        }

        /* global events */
        switch( event.type )
        {    
            case SDL_KEYDOWN:
                switch ( event.key.keysym.sym )
                {
                    /* temp */
                    case SDLK_EQUALS:
                        get_player(get_local_player_index())->current_anim->speed++;
                        break;
                    case SDLK_MINUS:
                        get_player(get_local_player_index())->current_anim->speed--;
                        break;
                    /* temp */
                    case SDLK_ESCAPE: /* Quit */
                        quit(0);
                        break;
                    case SDLK_s: /* Open server dialog */
                        if ( !get_server_active() && !get_client_active() ) 
                            show_server_dialog(FALSE);
                        break;
                    case SDLK_c: /* Open clients dialog */
                        if ( !get_server_active() && !get_client_active() )
                            show_client_dialog(FALSE);
                        break;
                    case SDLK_t: /* Open chat dialog */
                        if ( get_server_active() || get_client_active() )
                            show_chat_dialog(FALSE);
                        break;
                    case SDLK_e: /* Toggle edit mode */
                        if (!gg_dialog_current())
                        {
                            if (get_editing())
                                set_editing(FALSE);
                            else
                                set_editing(TRUE);
                        }
                        break;
                    default:
                        break;
                }
                break;

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
			case SDL_QUIT: /* handle quit requests */
			    quit(0);
			    break;
			default:
			    break;
		}
	}
}
