
#include "main.h"

layer *player_layer;
layer *obs_layer;

int get_tile( layer *lay, int xpos, int ypos )
{
    int where=(ypos*lay->width)+xpos;

    return lay->tiles[where];    
}

void set_tile( layer *lay, int xpos, int ypos, int index )
{
    int where=(ypos*lay->width)+xpos;

    if ( where >= 0 && where <= lay->width*lay->height )
        lay->tiles[where]=index;
}

void load_map( char *filename )
{
    /* temporary */
    player_layer = create_layer( 30, 20 );
    obs_layer = create_layer( 30, 20 );
}

layer *get_player_layer()
{
    return player_layer;
}

layer *get_obs_layer()
{
    return obs_layer;
}

layer *create_layer( int width, int height )
{
    layer *temp;
    int i;

    if ( width < 20 )
        width=20;
    if ( height < 15 )
        height=15;

    temp = malloc( sizeof(layer) );
    temp->tiles = malloc( width*height );
    temp->width=width;
    temp->height=height;

    for (i=0; i<width*height; i++ )
        temp->tiles[i]=8;

    return temp;    
}

void draw_layer( layer *lay )
{
    int i=0, j=0;

    for ( j=0; j<lay->width; j++ )
    {
        glPushMatrix();
        for ( i=0; i<lay->height; i++ )
        {
            draw_tile(lay->tiles[(i*lay->width)+j]);

            glPushMatrix();    
            glTranslatef( 0.0f, 10.0f, 0.0f );
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
