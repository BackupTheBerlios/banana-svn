
/*
    Banana - A multiplayer platform game engine.
    Copyright(C) 2006, Kris McAulay  

    tilebay.c - Load and draw tiles. 
*/

#include "main.h"

texture_t tilebay;

/* Loads the texture used as the tile bay */
void load_tilebay( char *filename )
{
    /* Load the texture using alpha, as it's needed for the foreground layer. */
    load_texture( &tilebay, filename, USE_ALPHA );
}

/* Draw a tile */
void draw_tile( int index )
{
    float u1,u2,v1,v2;
    /* Calculate how many tiles wide the tilebay's texture is */
    float tiles_per_row=(float)(tilebay.width/TILE_SIZE);

    /* Calculate the u/v value of the specific tile index */
    u1=(float)((index%(int)tiles_per_row)*TILE_SIZE)/tilebay.width;
    u2=(float)(((index%(int)tiles_per_row)*TILE_SIZE)+TILE_SIZE)/tilebay.width;
    v1=(float)((index/(int)tiles_per_row)*TILE_SIZE)/tilebay.height;    
    v2=(float)(((index/(int)tiles_per_row)*TILE_SIZE)+TILE_SIZE)/tilebay.height;  

    /* Enable textures */
    glEnable( GL_TEXTURE_2D );

    /* Bind the tilebay texture */
    glBindTexture( GL_TEXTURE_2D, tilebay.gl_index );

    /* Send the vertex/texture coordinates */
    glBegin( GL_QUADS ); 
        /* Top Left */
        glTexCoord2f( u1, v1 ); 
        glVertex3f( 0.0f,  0.0f, 0.0f );

        /* Top Right */
        glTexCoord2f( u2, v1 ); 
        glVertex3f( TILE_SIZE, 0.0f, 0.0f );

        /* Bottom Right */
        glTexCoord2f( u2, v2 ); 
        glVertex3f( TILE_SIZE, TILE_SIZE,  0.0f );

        /* Bottom Left */
        glTexCoord2f( u1, v2 ); 
        glVertex3f( 0.0f,  TILE_SIZE,  0.0f );
    glEnd(); 

    /* Disable textures */
    glDisable( GL_TEXTURE_2D );
}
