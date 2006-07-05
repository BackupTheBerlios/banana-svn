
#include "main.h"

static gg_colour_t col_black =
{
    0.0f, 0.0f, 0.0f, 1.0f
};

texture_t SDL_GL_LoadTexture(SDL_Surface *surface, SDL_Rect *area, int alpha);
texture_t framebuffer_grab;

texture_t *get_framebuffer_grab()
{
    return &framebuffer_grab;
}

void grab_framebuffer()
{
    char *data, *data2;
    int i=0;
    framebuffer_grab.width=1024;
    framebuffer_grab.height=512;
    int width=1024;
    int height=512; 
    int start_time,buffertime,inverttime,createtime;

    start_time=SDL_GetTicks();

    glDeleteTextures( 1, &framebuffer_grab.gl_index );

    data = malloc( 1024 * (512) * 3 );
    glReadPixels( 0, 0, 1024, 512, GL_RGB, GL_UNSIGNED_BYTE, data );
    buffertime=SDL_GetTicks()-start_time;
    printf( "Took %i ticks to read the buffer\n", buffertime );

    data2 = malloc( 1024 * (512) * 3);
    for ( i=0; i<512-32; i++ )
    {
        memcpy( data2 + ((512-33-i)*(width*3)), data + (i*(width*3)), width*3 );
    }
    inverttime=SDL_GetTicks()-start_time-buffertime;
    printf( "Took %i ticks to invert buffer\n", inverttime );

    glGenTextures(1, &framebuffer_grab.gl_index);

    glBindTexture(GL_TEXTURE_2D, framebuffer_grab.gl_index);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, 3, 1024, 512, 0, GL_RGB, GL_UNSIGNED_BYTE, data2);
    createtime=SDL_GetTicks()-start_time-buffertime-inverttime;
    printf( "Took %i ticks to create GL texture\n", createtime );

    printf( "Took a total of %i ticks.\n", SDL_GetTicks()-start_time );

    free(data);
}

static void draw_texture_uv( texture_t *texture, float xpos,
                             float ypos, float width, float height, float zpos,
                             gg_colour_t *col, float u1, float v1, float u2, float v2, GLenum mode_h, GLenum mode_v, float scale)
{
    glEnable( GL_TEXTURE_2D );

    glColor4f( col->r, col->g, col->b, col->a );
    //printf( "Scale: %f\n", scale );
    glBindTexture(GL_TEXTURE_2D, texture->gl_index);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, mode_h);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, mode_v);

    glBegin(GL_QUADS);
    glTexCoord2f(u1, v1);
    glVertex3f( xpos, ypos, zpos );
    glTexCoord2f(u2, v1);
    glVertex3f( xpos+width*scale, ypos, zpos );
    glTexCoord2f(u2, v2);
    glVertex3f( xpos+width*scale, ypos+height*scale, zpos );
    glTexCoord2f(u1, v2);
    glVertex3f( xpos, ypos+height*scale, zpos );
    glEnd();

    //glScalef( 1.5f, 1.5f, 1.5f );
    glDisable(GL_TEXTURE_2D);
}

void draw_texture(texture_t *texture, gg_rect_t source, gg_rect_t dest, int mode_h, int mode_v, gg_colour_t *colour, int effect )
{
    /*
        Effect 0 = none
        Effect 1 = shadow
    */

    float hsize = texture->u2 - texture->u1;
    float vsize = texture->v2 - texture->v1;
    float tex_h = texture->width / hsize;
    float tex_v = texture->height / vsize;
    float xsrc = texture->u1 + source.x / tex_h;
    float ysrc = texture->v1 + source.y / tex_v;
    float width, height;
    GLenum en_h, en_v;

    if (mode_h == GG_MODE_TILE)
    {
        en_h = GL_REPEAT;
        width = dest.width / tex_h;
    }
    else
    {
        en_h = GL_CLAMP;
        width = source.width / tex_h;
    }

    if (mode_v == GG_MODE_TILE)
    {
        en_v = GL_REPEAT;
        height = dest.height / tex_v;
    }
    else
    {
        en_v = GL_CLAMP;
        height = source.height / tex_v;
    }

    if (effect == 1)
    {
        glPushMatrix();
        glTranslatef( 2.0f, 2.0f, 0.0f );
        draw_texture_uv(texture, dest.x, dest.y, dest.width, dest.height, 0.0f, &col_black, 
            xsrc, ysrc, xsrc + width, ysrc + height, en_h, en_v, 1.0f);
        glPopMatrix();
    }

    glPushMatrix();
    draw_texture_uv(texture, dest.x, dest.y, dest.width, dest.height, 0.0f, colour, 
        xsrc, ysrc, xsrc + width, ysrc + height, en_h, en_v, 1.0f);
    glPopMatrix();
}

#ifdef FART
/*void draw_texture( texture_t *texture )
{
    glEnable( GL_TEXTURE_2D );
    glBindTexture( GL_TEXTURE_2D, texture->gl_index );

    /* Draw a tile.. */
    glBegin( GL_QUADS ); 
        glTexCoord2f( 0.0f, 0.0f ); /* Top Left */
        glVertex3f( 0.0f,  0.0f, 0.0f );

        glTexCoord2f( 1.0f, 0.0f ); /* Top Right */
        glVertex3f( texture->width, 0.0f, 0.0f );

        glTexCoord2f( 1.0f, 1.0f ); /* Bottom Right */
        glVertex3f( texture->width, texture->height,  0.0f );

        glTexCoord2f( 0.0f, 1.0f ); /* Bottom Left */
        glVertex3f( 0.0f,  texture->height,  0.0f );
    glEnd( ); 

    glDisable( GL_TEXTURE_2D );
}*/
#endif

void load_texture( texture_t *texture, char *filename, int alpha )
{
    /* Create storage space for the texture */
    SDL_Surface *texture_image;

    /* Load The Bitmap, Check For Errors, If Bitmap's Not Found Quit */
    if ( ( texture_image = IMG_Load( filename ) ) )
    {
        SDL_Rect area;
        area.x = 0;
        area.y = 0;
        area.w = texture_image->w;
        area.h = texture_image->h;
        *texture = SDL_GL_LoadTexture(texture_image, &area, alpha);
    }
    else
    {
        fprintf(stderr, "Could not load texture: %s!\n", filename);
        exit(1);
    }

    /* Free up any memory we may have used */
    if ( texture_image )
        SDL_FreeSurface( texture_image );
}

#ifdef FART
texture *load_texture( char *filename )
{
    texture *tex;
    tex=malloc( sizeof(texture) );
    
    /* Create storage space for the texture */
    SDL_Surface *texture_image;

    printf( "Loading texture: %s\n", filename );

    /* Load The Bitmap, Check For Errors, If Bitmap's Not Found Quit */
    if ( ( texture_image = IMG_Load( filename ) ) )
    {
        GLfloat textcoords[4];

        tex->width=texture_image->w;
        tex->height=texture_image->h;
        tex->gl_index = SDL_GL_LoadTexture(texture_image, textcoords);
    }
    else
    {
        fprintf(stderr, "Could not load texture: %s!\n", filename);
        exit(1);
    }

    printf( "Loaded texture: %s(%i,%i)\n", filename, tex->width, tex->height );

    /* Free up any memory we may have used */
    if ( texture_image )
        SDL_FreeSurface( texture_image );

    return tex;
}
#endif

static int power_of_two(int input)
{
	int value = 1;

	while ( value < input ) {
		value <<= 1;
	}
	return value;
}

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

    image = SDL_CreateRGBSurface(
                SDL_SWSURFACE,
                w, h,
                32,
#if SDL_BYTEORDER == SDL_LIL_ENDIAN /* OpenGL RGBA masks */
                0x000000FF,
                0x0000FF00,
                0x00FF0000,
                0xFF000000
#else
                0xFF000000,
                0x00FF0000,
                0x0000FF00,
                0x000000FF
#endif
            );
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

    /* Copy the surface into the GL texture image */
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

    /* Create an OpenGL texture for the image */
    glGenTextures(1, &texture.gl_index);
    glBindTexture(GL_TEXTURE_2D, texture.gl_index);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 (alpha ? 4 : 3),
                 w, h,
                 0,
                 GL_RGBA,
                 GL_UNSIGNED_BYTE,
                 image->pixels);
    SDL_FreeSurface(image); /* No longer needed */

    texture.u1 = 0;
    texture.v1 = 0;
    texture.u2 = area->w / (float) w;
    texture.v2 = area->h / (float) h;
    texture.width = area->w;
    texture.height = area->h;

    return texture;
}

#ifdef FART
GLuint SDL_GL_LoadTexture(SDL_Surface *surface, GLfloat *texcoord)
{
	GLuint texture;
	int w, h;
	SDL_Surface *image;
	SDL_Rect area;
	Uint32 saved_flags;
	Uint8  saved_alpha;

	/* Use the surface width and height expanded to powers of 2 */
	w = power_of_two(surface->w);
	h = power_of_two(surface->h);
	texcoord[0] = 0.0f;			/* Min X */
	texcoord[1] = 0.0f;			/* Min Y */
	texcoord[2] = (GLfloat)surface->w / w;	/* Max X */
	texcoord[3] = (GLfloat)surface->h / h;	/* Max Y */

	image = SDL_CreateRGBSurface(
			SDL_SWSURFACE,
			w, h,
			32,
#if SDL_BYTEORDER == SDL_LIL_ENDIAN /* OpenGL RGBA masks */
			0x000000FF, 
			0x0000FF00, 
			0x00FF0000, 
			0xFF000000
#else
			0xFF000000,
			0x00FF0000, 
			0x0000FF00, 
			0x000000FF
#endif
		       );
	if ( image == NULL ) {
		return 0;
	}

	/* Save the alpha blending attributes */
	saved_flags = surface->flags&(SDL_SRCALPHA|SDL_RLEACCELOK);
	saved_alpha = surface->format->alpha;
	if ( (saved_flags & SDL_SRCALPHA) == SDL_SRCALPHA ) {
		SDL_SetAlpha(surface, 0, 0);
	}

	/* Copy the surface into the GL texture image */
	area.x = 0;
	area.y = 0;
	area.w = surface->w;
	area.h = surface->h;
	SDL_BlitSurface(surface, &area, image, &area);

	/* Restore the alpha blending attributes */
	if ( (saved_flags & SDL_SRCALPHA) == SDL_SRCALPHA ) {
		SDL_SetAlpha(surface, saved_flags, saved_alpha);
	}

	/* Create an OpenGL texture for the image */
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexImage2D(GL_TEXTURE_2D,
		     0,
		     GL_RGBA,
		     w, h,
		     0,
		     GL_RGBA,
		     GL_UNSIGNED_BYTE,
		     image->pixels);
	SDL_FreeSurface(image); /* No longer needed */

	return texture;
}
#endif

