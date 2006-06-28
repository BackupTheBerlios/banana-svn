
#include "main.h"

void draw_tile( int index )
{
    float u1,u2,v1,v2;
    float tiles_per_row=(float)(get_tilebay()->width/TILE_SIZE);

    u1=index%(int)tiles_per_row; u1=u1*TILE_SIZE;
    u2=u1+TILE_SIZE;
    v1=index/(int)tiles_per_row; v1=v1*TILE_SIZE;    
    v2=v1+TILE_SIZE;

    /* printf( "u:(%f,%f) v:(%f,%f)\n", u1, u2, v1, v2 ); */

    u1=u1/get_tilebay()->width;
    u2=u2/get_tilebay()->width;
    v1=v1/get_tilebay()->height;
    v2=v2/get_tilebay()->height;

    /* printf( "u:(%f,%f) v:(%f,%f)\n", u1, u2, v1, v2 ); */

    glEnable( GL_TEXTURE_2D );
    glBindTexture( GL_TEXTURE_2D, get_tilebay()->gl_index );

    /* Draw a tile.. */
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

