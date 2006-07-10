
/*
    Banana - A multiplayer platform game engine.
    Copyright(C) 2006, Kris McAulay  

    text.c - Load fonts. 
*/

#include "main.h"

font_t font;

/* Return a pointer to the font */
font_t *get_font()
{
    return &font;
}

/* TEMPORARY -- Move to XML ------------------------------------------- */
static void set_font_widths( char *filename )
{
  int temp;
  FILE * file;
  int i=0;

  file = fopen (filename,"r");
  
  if ( file == NULL )
  {
    printf( "Error opening %s\n", filename );
    quit(1);
  }

  while ( fscanf( file, "%i", &temp ) != EOF )
  { 
    if ( i == 0 ) /* First entry.. height.. */
        font.height=temp;
    else
        font.widths[i-1]=temp;
   
  /*  printf( "Scanned, and got %i for %i\n", temp, i-1 );*/
    i++;
  }

  fclose (file); 
}
/* TEMPORARY -- Move to XML ------------------------------------------- */

/* Load a font. */
void load_font( char *texfile, char *widfile )
{
    int i=0;
    SDL_Rect rect;

    SDL_Surface *surface;

    /* Load a font width file into the widths array */
    set_font_widths( widfile );

    /* Try to load the font image */
    if ((surface = IMG_Load(texfile)))
    {
        /* Create individual textures in the character array for each character */
        for (i=0;i<95;i++)
        {
            /* Calculate the pixel location of the individual character */
            rect.x = (i%(surface->w/font.height))*(font.height);
            rect.y = (i/(surface->w/font.height))*(font.height);
            rect.w = font.widths[i];
            rect.h = font.height;
            /* Create the texture from the surface using the above coordinates. */
            font.characters[i]=SDL_GL_LoadTexture(surface, &rect, 1);      
        }
        /* We don't need the surface anymore. Free it */        
        SDL_FreeSurface(surface);
    }
    else
    {
        fprintf(stderr, "Could not load texture: '%s'!\n", texfile);
        quit(1);
    }
}
