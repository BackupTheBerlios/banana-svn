
#include "main.h"

texture_t mouse_cursor;

int mouse_x, mouse_y;
int mouse_buttons[3];

void update_mouse()
{
    int i=0;
    SDL_PumpEvents();

    for (i=0; i<3; i++)
    {
        mouse_buttons[i]=SDL_GetMouseState(&mouse_x, &mouse_y) & SDL_BUTTON(i+1);
    }
    mouse_y=479-mouse_y;
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
        glVertex3f( 0.0f,  1.0f, 0.0f );

        glTexCoord2f( 1.0f, 0.0f ); /* Top Right */
        glVertex3f( TILE_SIZE, 1.0f, 0.0f );

        glTexCoord2f( 1.0f, 1.0f ); /* Bottom Right */
        glVertex3f( TILE_SIZE, 1.0-TILE_SIZE,  0.0f );

        glTexCoord2f( 0.0f, 1.0f ); /* Bottom Left */
        glVertex3f( 0.0f,  1.0-TILE_SIZE,  0.0f );
    glEnd( ); 

    glDisable( GL_BLEND );
    glDisable( GL_TEXTURE_2D );    
}

int get_mouse_button( int button )
{
    int i=0, retval=FALSE;

    if ( button == BUTTON_ANY || button == BUTTON_WHICH )
    {
        for (i=0; i<3; i++ )
        {
            if ( mouse_buttons[i] )
            {
                if ( button == BUTTON_WHICH )    
                    retval=i;
                else
                    retval=TRUE;
            }
        }
    }
    else
        retval=mouse_buttons[button];

    return retval;
}

int get_mouse_x()
{  
    return mouse_x;
}

int get_mouse_y()
{   
    return mouse_y;
}
