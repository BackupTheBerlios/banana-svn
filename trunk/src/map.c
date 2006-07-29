
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
    player_layer = create_layer( 50, 50 );
    obs_layer = create_layer( 50, 50 );

    player_layer->tiles[0]=3;
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
        temp->tiles[i]=0;

    return temp;    
}

void save_map( char *filename )
{
    FILE *f;
    f=fopen( filename, "wb" );


    fclose(f);
}

void draw_layer( layer *lay )
{
    int x=0, y=0;
    int tiles_wide=SCREEN_WIDTH/TILE_SIZE;
    int tiles_high=SCREEN_HEIGHT/TILE_SIZE;

    for (x=(get_camera_x()/TILE_SIZE);x<(get_camera_x()/TILE_SIZE)+tiles_wide;x++)
    {
        glPushMatrix();
        for ( y=(get_camera_y()/TILE_SIZE); y<(get_camera_y()/TILE_SIZE)+tiles_high; y++ )
        {
            draw_tile(lay->tiles[(y*lay->width)+x]);
            glTranslatef( 0.0f, 32.0f, 0.0f );
        }
        glPopMatrix();
        glTranslatef( 32.0f, 0.0f, 0.0f );
    }
}

void draw_map()
{
    glTranslatef( -(int)get_camera_x()%32, -(int)get_camera_y()%32, 0.0f );
    draw_layer( get_player_layer() );
}
