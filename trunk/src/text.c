
#include "main.h"

font_t *big_font;
font_t *small_font;

void set_font_widths( font_t *f, char *filename );

font_t *get_big_font()
{
    return big_font;
}

font_t *get_small_font()
{
    return small_font;
}

font_t *load_font( char *texfile, char *widfile )
{
    int i=0;
    SDL_Rect rect;
    font_t *temp;

    SDL_Surface *surface;

    temp=malloc(sizeof(font_t));
    set_font_widths( temp, widfile );

    if ((surface = IMG_Load(texfile)))
    {
        for (i=0;i<95;i++)
        {
            rect.x = (i%(surface->w/temp->size))*(temp->size);
            rect.y = (i/(surface->w/temp->size))*(temp->size);
            rect.w = temp->widths[i];
            rect.h = temp->size;
            temp->characters[i]=SDL_GL_LoadTexture(surface, &rect, 1);      
        }
        SDL_FreeSurface(surface);
    }
    else
    {
        fprintf(stderr, "Could not load texture: %s!\n", texfile);
        exit(1);
    }

    return temp;
}

void load_fonts()
{
    big_font=load_font( "data/big_font.png", "data/big_font.wid" );
    small_font=load_font( "data/small_font.png", "data/small_font.wid" );
}

int get_text_size( font_t *f )
{
    return f->size;
}

int get_text_len( font_t *f, char *string )
{
    int i;
    int length=0;
    int index;

    for ( i=0; i<strlen(string); i++ )
    {
        index=string[i];
        if ( string[i] == ' ' )
            length+=7.0f;
        else
            length+=f->widths[index-32]+1.0f;
    }

    return length;
}

#ifdef FART
void get_string_size(char *s, int *width, int *height)
{
    int i;

    if (width)
        *width = 0;
    if (height)
        *height = 0;

    for (i = 0; i < strlen(s); i++)
    {
        int c_width, c_height;
        gui_get_char_size(s[i], &c_width, &c_height);

        if (width)
            *width += c_width;
        if (height && (c_height > *height))
            *height = c_height;
    }
}

void draw_string( font_t *f, char *s, int x, int y, gg_colour_t *colour, int bounce, int effect, float align)
{
    int i;
    unsigned int ticks = gg_system_get_ticks();
    gg_rect_t rect_d;
    rect_d.x = x;

    if (align != 0.0f)
    {
        int width;

        get_string_size(s, &width, NULL);

        rect_d.x -= width * align;
    }

    for (i = 0; i < strlen((char *) s); i++)
    {
        int y_off = 0;
        texture_t *image = gui_get_char_image(s[i]);
        gg_rect_t rect_s = {0, 0};

        if (bounce)
        {
            float phase = ((ticks % (1000 / GG_BOUNCE_SPEED)) / (float) (1000 / GG_BOUNCE_SPEED));

            if (phase < 0.5)
                y_off = phase * 2 * (GG_BOUNCE_AMP + 1);
            else
                y_off = ((1.0 - phase) * 2) * (GG_BOUNCE_AMP + 1);
        }

        gg_system_get_image_size(image, &rect_s.width, &rect_s.height);
       // printf( "Char i:%c=%i,%i\n", s[i], rect_s.width, rect_s.height );
        rect_d.width = rect_s.width;
        rect_d.height = rect_s.height;
        rect_d.y = y + y_off;
        gg_system_draw_image(image, rect_s, rect_d, GG_MODE_SCALE, GG_MODE_SCALE, colour, effect);
        rect_d.x += rect_s.width+1.0f;

        ticks += 1000 / GG_BOUNCE_SPEED / GG_BOUNCE_LEN;
    }
}
#endif

#ifdef FARTING
void draw_char( font_t *f, char c )
{
    int index=0;

    if ( c > 32 && c < 127 )    
    {
    index = c-32;

    printf( "OMGChar i:%c=%i,%i\n", index, f->characters[index].width, f->characters[index].height );

    glEnable( GL_TEXTURE_2D );
    glBindTexture( GL_TEXTURE_2D, f->characters[index].gl_index );

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glBegin( GL_QUADS ); 
        glTexCoord2f( 0.0f, 0.0f ); /* Top Left */
        glVertex3f( 0.0f,  0.0f, 0.0f );

        glTexCoord2f( 1.0f, 0.0f ); /* Top Right */
        glVertex3f( f->characters[index].width, 0.0f, 0.0f );

        glTexCoord2f( 1.0f, 1.0f ); /* Bottom Right */
        glVertex3f( f->characters[index].width, f->characters[index].height,  0.0f );

        glTexCoord2f( 0.0f, 1.0f ); /* Bottom Left */
        glVertex3f( 0.0f,  f->characters[index].height,  0.0f );
    glEnd( ); 

    glDisable( GL_BLEND );
    glDisable( GL_TEXTURE_2D );
    }
}

void draw_string( font_t *f, char *text )
{
    int i=0;
    int index=0;
    int new_len=0;
    int done=FALSE;

    glPushMatrix();
    for ( i=0; i<strlen(text); i++ )
    {
        draw_char( f, text[i] ); 
        glTranslatef( f->widths[text[i]-32], 0.0f, 0.0f );

        //index=text[i]-32;   

        /*if ( text[i] == ' ' )
        {
            glTranslatef( 7.0f, 0.0f, 0.0f );
            new_len+=7.0f;
        }
        else*/
        {

          //  new_len+=f->widths[i]+1.0f;
        }
        
       // if ( new_len+f->widths[index+1] > len )
       //     done=TRUE;

       /* printf( "length: %i, new length: %i\n", len, new_len );*/
    }
    glPopMatrix();

    return i;
}
#endif

/*void draw_string( font_t *f, char *text )
{
    draw_partial_string( f, text, 0, get_text_len(get_small_font(),text) );
}*/

/* temporary.. move to XML later.. */
void set_font_widths( font_t *f, char *filename )
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
        f->size=temp;
    else
        f->widths[i-1]=temp;
   
  /*  printf( "Scanned, and got %i for %i\n", temp, i-1 );*/
    i++;
  }

  fclose (file); 
}
