
#include "main.h"

texture_t mouse_cursor;

void process_mouse()
{
    SDL_PumpEvents(); 

    if( SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(1) )
    {        
        set_tile(get_player_layer(), (get_camera_x()+get_mouse_x())/32, (get_camera_y()+get_mouse_y())/32, 1);

        /* TEMporary!!!*/
        net_change_tile( (get_camera_x()+get_mouse_x())/32, (get_camera_y()+get_mouse_y())/32 );
        /* ----^ Temporary!! */
    }    
}

void load_mouse_cursor( char *filename )
{
    SDL_ShowCursor(0);
    load_texture( &mouse_cursor, filename, 1 );
}

void draw_mouse_cursor()
{
    glEnable( GL_TEXTURE_2D );
    glBindTexture( GL_TEXTURE_2D, mouse_cursor.gl_index );

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glBegin( GL_QUADS ); 
        glTexCoord2f( 0.0f, 0.0f ); /* Top Left */
        glVertex3f( 0.0f,  0.0f, 0.0f );

        glTexCoord2f( 1.0f, 0.0f ); /* Top Right */
        glVertex3f( TILE_SIZE, 0.0f, 0.0f );

        glTexCoord2f( 1.0f, 1.0f ); /* Bottom Right */
        glVertex3f( TILE_SIZE, TILE_SIZE,  0.0f );

        glTexCoord2f( 0.0f, 1.0f ); /* Bottom Left */
        glVertex3f( 0.0f,  TILE_SIZE,  0.0f );
    glEnd( ); 

    glDisable( GL_BLEND );
    glDisable( GL_TEXTURE_2D );    
}

int get_mouse_x()
{
    int temp;

    SDL_PumpEvents();    
    SDL_GetMouseState(&temp, NULL);
    
    return temp;
}

int get_mouse_y()
{
    int temp;

    SDL_PumpEvents();    
    SDL_GetMouseState(NULL, &temp);
    
    return temp;
}
