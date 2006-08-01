
#include "main.h"

layer_t *player_layer;
layer_t *obs_layer;

int get_tile( layer_t *lay, int xpos, int ypos )
{
    int where=(ypos*lay->width)+xpos;

    return lay->tiles[where];    
}

void set_tile( layer_t *lay, int xpos, int ypos, int index )
{
    int where=(ypos*lay->width)+xpos;

    if ( where >= 0 && where <= lay->width*lay->height )
        lay->tiles[where]=index;
}

/*void load_map( char *filename )
{
    player_layer = create_layer( 211, 50 );
    obs_layer = create_layer( 211, 50 );
}*/

layer_t *get_player_layer()
{
    return player_layer;
}

layer_t *get_obs_layer()
{
    return obs_layer;
}

layer_t *create_layer( int width, int height )
{
    layer_t *temp;
    int i;

    if ( width < 20 )
        width=20;
    if ( height < 15 )
        height=15;

    temp = malloc( sizeof(layer_t) );
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

    fwrite(&player_layer->width, sizeof(int), 1, f);
    fwrite(&player_layer->height, sizeof(int), 1, f);
    fwrite(player_layer->tiles, sizeof(int), player_layer->width*player_layer->height, f);
    fwrite(obs_layer->tiles, sizeof(char), player_layer->width*player_layer->height, f);

    fclose(f);
}

void load_map( char *filename )
{
    FILE *f;
    f=fopen( filename, "rb" );

    player_layer=malloc(sizeof(layer_t));
    fread(&player_layer->width, sizeof(int), 1, f);
    fread(&player_layer->height, sizeof(int), 1, f);
    
    /*printf( "Reading map %s[%i,%i].\n", filename, player_layer->width, player_layer->height );*/

    player_layer->tiles=malloc(player_layer->width*player_layer->height);
    fread(player_layer->tiles, sizeof(char), player_layer->width*player_layer->height, f);

    obs_layer=malloc(sizeof(layer_t));
    obs_layer->tiles=malloc(player_layer->width*player_layer->height);
    fread(obs_layer->tiles, sizeof(char), player_layer->width*player_layer->height, f);

    fclose(f);
}

void draw_layer( layer_t *lay )
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
            glTranslatef( 0.0f, TILE_SIZE, 0.0f );
        }
        glPopMatrix();
        glTranslatef( TILE_SIZE, 0.0f, 0.0f );
    }
}

void draw_map()
{
    glTranslatef( -(int)get_camera_x()%TILE_SIZE, -(int)get_camera_y()%TILE_SIZE, 0.0f );
    draw_layer( get_player_layer() );
}
