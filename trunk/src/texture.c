
/*
    Banana - A multiplayer platform game engine.
    Copyright(C) 2006, Kris McAulay  

    texture.c - Load textures, grab framebuffer. 
*/

#include "main.h"

texture_t framebuffer_grab;

/* Return the nearest power of two greater than the input */
static int power_of_two(int input)
{
	int value = 1;

	while ( value < input ) {
		value <<= 1;
	}
	return value;
}

/* Return the framebuffer texture */
texture_t *get_framebuffer_grab()
{
    return &framebuffer_grab;
}

/* Update framebuffer texture */
void grab_framebuffer()
{
    char *data, *data2;
    int width, height; 
    int i=0;

    /* Find the smallest ^2 size that can fit the screen. */
    width=power_of_two(SCREEN_WIDTH);
    height=power_of_two(SCREEN_HEIGHT);

    /* Set the framebuffer texture's size */
    framebuffer_grab.width=width;
    framebuffer_grab.height=height;

    /* Delete the OpenGL texture so we can create a new one */
    glDeleteTextures( 1, &framebuffer_grab.gl_index );

    /* Allocate memory for the pixel buffer */
    data = malloc( width * (height) * 3 );

    /* Read the framebuffer into the buffer */
    glReadPixels( 0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, data );

    /* Allocate memory for a second buffer */
    data2 = malloc( width * (height) * 3);

    /* Use the second buffer to flip the buffers Y axis */
    for ( i=0; i<height-32; i++ )
    {
        memcpy( data2 + ((height-33-i)*(width*3)), data + (i*(width*3)), width*3 );
    }
    
    /* Create a GL texture from the now flipped buffer */
    glGenTextures(1, &framebuffer_grab.gl_index);
    glBindTexture(GL_TEXTURE_2D, framebuffer_grab.gl_index);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data2);

    /* Free the buffers */
    free(data);
    free(data2);
}

/* Create a texture from an SDL surface */
texture_t SDL_GL_LoadTexture(SDL_Surface *surface, SDL_Rect *area, int alpha)
{
    texture_t texture;
    int w, h;
    SDL_Surface *image;
    SDL_Rect dest;
    Uint32 saved_flags;
    Uint8  saved_alpha;

    /* Use the surface width and height expanded to powers of 2 */
    w = power_of_two(area->w);
    h = power_of_two(area->h);

    /* Create a blank SDL surface */
    image = SDL_CreateRGBSurface( SDL_SWSURFACE, w, h, 32,
        #if SDL_BYTEORDER == SDL_LIL_ENDIAN 
            0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000 );
        #else
            0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF );
        #endif

    /* Check if the surface was created */
    if ( image == NULL )
    {
        exit(0);
    }

    /* Save the alpha blending attributes */
    saved_flags = surface->flags&(SDL_SRCALPHA|SDL_RLEACCELOK);
    saved_alpha = surface->format->alpha;
    if ( (saved_flags & SDL_SRCALPHA) == SDL_SRCALPHA )
    {
        SDL_SetAlpha(surface, 0, 0);
    }

    /* Blit the SDL surface into a temporary surface */
    dest.x = 0;
    dest.y = 0;
    dest.w = area->w;
    dest.h = area->h;
    SDL_BlitSurface(surface, area, image, &dest);

    /* Restore the alpha blending attributes */
    if ( (saved_flags & SDL_SRCALPHA) == SDL_SRCALPHA )
    {
        SDL_SetAlpha(surface, saved_flags, saved_alpha);
    }

    /* Create an OpenGL texture from the temporary surface. */
    glGenTextures(1, &texture.gl_index);
    glBindTexture(GL_TEXTURE_2D, texture.gl_index);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, (alpha ? 4 : 3), w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image->pixels);
    SDL_FreeSurface(image);

    /* Set up the texture */
    texture.u1 = 0;
    texture.v1 = 0;
    texture.u2 = area->w / (float) w;
    texture.v2 = area->h / (float) h;
    texture.width = area->w;
    texture.height = area->h;

    /* Return the texture */
    return texture;
}

/* Load a texture from a file */
void load_texture( texture_t *texture, char *filename, int alpha )
{
    SDL_Surface *texture_image;

    /* Load the texture file into an SDL surface */
    if ( ( texture_image = IMG_Load( filename ) ) )
    {
        /* Specify image size for conversion */
        SDL_Rect area;
        area.x = 0; area.y = 0;
        area.w = texture_image->w;
        area.h = texture_image->h;

        /* Convert SDL Surface to a texture */
        *texture = SDL_GL_LoadTexture(texture_image, &area, alpha);
    }
    else
    {
        fprintf(stderr, "Could not load texture: '%s'\n", filename);
        quit(1);
    }

    /* Free up any memory we may have used */
    if ( texture_image )
        SDL_FreeSurface( texture_image );
}
