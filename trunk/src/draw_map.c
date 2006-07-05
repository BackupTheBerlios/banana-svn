
#include "main.h"

void draw_layer( layer *lay )
{
    int i=0, j=0;
    char temp[10];

    for ( j=0; j<lay->width; j++ )
    {
        glPushMatrix();
        for ( i=0; i<lay->height; i++ )
        {
            draw_tile(lay->tiles[(i*lay->width)+j]);

            glPushMatrix();    
            //sprintf( temp, "%i", j ); draw_string(get_small_font(),temp, 0, 0, &col_black, 0, 0, 0);
            glTranslatef( 0.0f, 10.0f, 0.0f );
            //sprintf( temp, "%i", i ); draw_string(get_small_font(),temp, 0, 0, &col_black, 0, 0, 0);
            glPopMatrix();

            glTranslatef( 0.0f, 32.0f, 0.0f );
        }
        glPopMatrix();
        glTranslatef( 32.0f, 0.0f, 0.0f );
    }
}

void draw_map()
{
    draw_layer( get_player_layer() );
}
