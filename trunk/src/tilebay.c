/*
    Banana - A multiplayer platform game engine.
    Copyright(C) 2006, Kris McAulay    
*/

#include "main.h"

texture_t tilebay;

/* Loads the texture used as the tile bay */
void load_tilebay( char *filename )
{
    /* Load the texture using alpha, needed for the foreground layer. */
    load_texture( &tilebay, filename, USE_ALPHA );
}

/* Draw a tile, centered */
void draw_tile( int index )
{
    float u1,u2,v1,v2;
    float tiles_per_row=(float)(tilebay.width/TILE_SIZE);

    float u1n=0.0f;

    u1n=(float)((index%(int)tiles_per_row)*TILE_SIZE)/tilebay.width;

    /* Calculate the u/v value of the specific tile index */
    u1=index%(int)tiles_per_row; u1=u1*TILE_SIZE;
    u2=u1+TILE_SIZE;
    v1=index/(int)tiles_per_row; v1=v1*TILE_SIZE;    
    v2=v1+TILE_SIZE;

    u1=u1/tilebay.width;
    u2=u2/tilebay.width;
    v1=v1/tilebay.height;
    v2=v2/tilebay.height;

    //printf( "u1: %f, u1: %f\n", u1, u1n );

    u1=u1n;

    glEnable( GL_TEXTURE_2D );
    glBindTexture( GL_TEXTURE_2D, tilebay.gl_index );

    glBegin( GL_QUADS ); 
        glTexCoord2f( u1, v1 ); /* Top Left */
        glVertex3f( 0.0f,  0.0f, 0.0f );

        glTexCoord2f( u2, v1 ); /* Top Right */
        glVertex3f( TILE_SIZE, 0.0f, 0.0f );

        glTexCoord2f( u2, v2 ); /* Bottom Right */
        glVertex3f( TILE_SIZE, TILE_SIZE,  0.0f );

        glTexCoord2f( u1, v2 ); /* Bottom Left */
        glVertex3f( 0.0f,  TILE_SIZE,  0.0f );
    glEnd( ); 

    glDisable( GL_TEXTURE_2D );
}
