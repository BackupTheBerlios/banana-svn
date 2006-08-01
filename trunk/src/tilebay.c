
/*
    Banana - A multiplayer platform game engine.
    Copyright(C) 2006, Kris McAulay  

    tilebay.c - Load and draw tiles. 
*/

#include "main.h"

texture_t *tilebay;
int tile_count=0;

int get_tile_count()
{
    return tile_count;
}

/* Return a pointer to a tile */
texture_t *get_tile_tex( int index )
{
    return &tilebay[index];
}

/* Return a pointer to the tilebay array */
texture_t *get_tilebay_addr()
{
    return tilebay;
}

/* Loads the texture used as the tile bay */
void load_tilebay( char *filename )
{ 
    int i=0;
    SDL_Rect rect;
    SDL_Surface *surface;

    /* Try to load the font image */
    if ((surface = IMG_Load(filename)))
    {
        tile_count=(surface->w/32)*(surface->h/32);

        tilebay=malloc(sizeof(texture_t)*tile_count);

        /* Create individual textures in the character array for each character */
        for (i=0;i<((surface->w/32)*(surface->h/32));i++)
        {
            /* Calculate the pixel location of the individual character */
            rect.x = (i%(surface->w/32))*(32);
            rect.y = (i/(surface->w/32))*(32);
            rect.w = 32; rect.h = 32;
            /* Create the texture from the surface using the above coordinates. */
            tilebay[i]=SDL_GL_LoadTexture(surface, &rect, 1);      
        }
        tile_count=i;
        printf( "Tile count.. %i\n", tile_count );

        /* We don't need the surface anymore. Free it */        
        SDL_FreeSurface(surface);
    }
    else
    {
        fprintf(stderr, "Could not load texture: '%s'!\n", filename);
        quit(1);

    }

    /* Load the texture using alpha, as it's needed for the foreground layer. */
    /*load_texture( temp_tex, filename, USE_ALPHA );

    tilebay=malloc(sizeof(texture_t)*(tilebay->width*tilebay->height));*/
}

/* Draw a tile */
void draw_tile( int index )
{
    if ( index > get_tile_count() )
        return;

    /* Enable textures */
    glEnable( GL_TEXTURE_2D );

    /* Bind the tilebay texture */
    glBindTexture( GL_TEXTURE_2D, tilebay[index].gl_index );

    /* Send the vertex/texture coordinates */
    glBegin( GL_QUADS ); 
        /* Top Left */
        glTexCoord2f( 0.0f, 1.0f ); 
        glVertex3f( 0.0f,  0.0f, 0.0f );

        /* Top Right */
        glTexCoord2f( 1.0f, 1.0f ); 
        glVertex3f( TILE_SIZE, 0.0f, 0.0f );

        /* Bottom Right */
        glTexCoord2f( 1.0f, 0.0f ); 
        glVertex3f( TILE_SIZE, TILE_SIZE,  0.0f );

        /* Bottom Left */
        glTexCoord2f( 0.0f, 0.0f ); 
        glVertex3f( 0.0f,  TILE_SIZE,  0.0f );
    glEnd(); 

    /* Disable textures */
    glDisable( GL_TEXTURE_2D );
}
