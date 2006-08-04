
#include "main.h"

gg_dialog_style_t style_menu;
texture_t menu_border[9];
texture_t widget_images[8];

int gui_transition=FALSE;
float gui_transition_inc=0.0f;

static gg_colour_t col_black =
{
    0.0f, 0.0f, 0.0f, 1.0f
};

/* Get the length of a string of text */
int get_text_len( char *string )
{
    int i;
    int length=0;
    int index;

    /* For each character in the string, add it's width plus 1 for spacing. */
    for ( i=0; i<strlen(string); i++ )
    {
        index=string[i];
        /* If character is a space, add extra width */
        if ( string[i] == ' ' )
            length+=7.0f;
        else
            length+=get_font()->widths[index-32]+1.0f;
    }

    return length;
}

static void draw_texture_uv( texture_t *texture, float xpos,
                             float ypos, float width, float height, float zpos,
                             gg_colour_t *col, float u1, float v1, float u2, float v2, GLenum mode_h, GLenum mode_v, float scale)
{
    glEnable( GL_TEXTURE_2D );

    glColor4f( col->r, col->g, col->b, col->a-(1.0f-gg_dialog_trans()) );
    glBindTexture(GL_TEXTURE_2D, texture->gl_index);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, mode_h);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, mode_v);

    glBegin(GL_QUADS);
    glTexCoord2f(u1, v2);
    glVertex3f( xpos, ypos, zpos );
    glTexCoord2f(u2, v2);
    glVertex3f( xpos+width*scale, ypos, zpos );
    glTexCoord2f(u2, v1);
    glVertex3f( xpos+width*scale, ypos+height*scale, zpos );
    glTexCoord2f(u1, v1);
    glVertex3f( xpos, ypos+height*scale, zpos );
    glEnd();

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
        glTranslatef( 2.0f, -2.0f, 0.0f );
        draw_texture_uv(texture, dest.x, dest.y, dest.width, dest.height, 0.0f, &col_black, 
            xsrc, ysrc, xsrc + width, ysrc + height, en_h, en_v, 1.0f);
        glPopMatrix();
    }

    glPushMatrix();
    draw_texture_uv(texture, dest.x, dest.y, dest.width, dest.height, 0.0f, colour, 
        xsrc, ysrc, xsrc + width, ysrc + height, en_h, en_v, 1.0f);
    glPopMatrix();
}

void draw_glow( int x, int y, int width, int height, int glow_width, gg_colour_t *col, int middle )
{
    gg_colour_t solid, trans;

    solid.r=col->r; solid.g=col->g; solid.b=col->b; solid.a=1.0f;
    trans.r=col->r; trans.g=col->g; trans.b=col->b; trans.a=0.0f;

    /* Top */
    gg_system_draw_gradient_rect( x, y-glow_width, width, glow_width, &trans, &trans, &solid, &solid, FALSE);
    /* Bottom */
    gg_system_draw_gradient_rect( x, y+height, width, glow_width, &solid, &solid, &trans, &trans, FALSE);
    /* Left */
    gg_system_draw_gradient_rect( x-glow_width, y, glow_width, height, &trans, &solid, &trans, &solid, FALSE );
    /* Right */
    gg_system_draw_gradient_rect( x+width, y, glow_width, height, &solid, &trans, &solid, &trans, FALSE );
    
    /* Top left corner. */
    gg_system_draw_gradient_rect( x-glow_width, y-glow_width, glow_width, glow_width, 
        &trans, &trans, &trans, &solid, FALSE);
    /* Top right corner. */
    gg_system_draw_gradient_rect( x+width, y-glow_width, glow_width, glow_width, 
        &trans, &trans, &solid, &trans, TRUE);
    /* Bottom right corner. */
    gg_system_draw_gradient_rect( x+width, y+height, glow_width, glow_width, 
        &solid, &trans, &trans, &trans, FALSE);
    /* Bottom left corner. */
    gg_system_draw_gradient_rect( x-glow_width, y+height, glow_width, glow_width, 
        &trans, &solid, &trans, &trans, TRUE);       

    /* Middle */
    if (middle)
        gg_system_draw_gradient_rect( x, y, width, height, &solid, &solid, &solid, &solid, FALSE); 
}

static void load_widget_images(texture_t border[8], char *filename)
{
    /* Create storage space for the texture */
    SDL_Surface *surface;

    /* Load The Bitmap, Check For Errors, If Bitmap's Not Found Quit */
    if ((surface = IMG_Load(filename)))
    {
        int i;
        for (i = 0; i < 8; i++)
        {
            SDL_Rect rect;
            rect.x = (i % 4) * surface->w / 4;
            rect.y = (i / 4) * surface->h / 2;
            rect.w = surface->w / 4;
            rect.h = surface->h / 2;
            border[i] = SDL_GL_LoadTexture(surface, &rect, 1);
        }
        /* Free up any memory we may have used */
        SDL_FreeSurface(surface);
    }
    else
    {
        fprintf(stderr, "Could not load texture: %s!\n", filename);
        exit(1);
    }
}

static void load_border(texture_t border[9], char *filename)
{
    /* Create storage space for the texture */
    SDL_Surface *surface;

    /* Load The Bitmap, Check For Errors, If Bitmap's Not Found Quit */
    if ((surface = IMG_Load(filename)))
    {
        int i;
        for (i = 0; i < 9; i++)
        {
            SDL_Rect rect;
            rect.x = (i % 3) * surface->w / 3;
            rect.y = (i / 3) * surface->h / 3;
            rect.w = surface->w / 3;
            rect.h = surface->h / 3;
            border[i] = SDL_GL_LoadTexture(surface, &rect, 1);
        }
        /* Free up any memory we may have used */
        SDL_FreeSurface(surface);
    }
    else
    {
        fprintf(stderr, "Could not load texture: %s!\n", filename);
        exit(1);
    }
}

gg_dialog_style_t *get_menu_style()
{
    return &style_menu;
}

void gui_draw_rect(int x, int y, int w, int h, gg_colour_t *col)
{
    glColor4f(col->r, col->g, col->b, col->a-(1.0f-gg_dialog_trans()));
    glBegin(GL_LINE_LOOP);
    glVertex3f(x + 0.5f, y + 0.5f, 1.0f);
    glVertex3f(x + w + 0.5f, y + 0.5f, 1.0f);
    glVertex3f(x + w + 0.5f, y + h + 0.5f, 1.0f);
    glVertex3f(x + 0.5f, y + h + 0.5f, 1.0f);
    glEnd();
}

/*void draw_rect(int x, int y, int w, int h, gg_colour_t *col)
{
    glColor4f(col->r, col->g, col->b, col->a);
    glBegin(GL_LINE_LOOP);
    glVertex3f(x + 0.5f, y + 0.5f, 1.0f);
    glVertex3f(x + w + 0.5f, y + 0.5f, 1.0f);
    glVertex3f(x + w + 0.5f, y + h + 0.5f, 1.0f);
    glVertex3f(x + 0.5f, y + h + 0.5f, 1.0f);
    glEnd();
}*/

void gui_draw_rect_fill(int x, int y, int w, int h, 
    gg_colour_t *top_left, gg_colour_t *top_right, gg_colour_t *bottom_left, gg_colour_t *bottom_right, int start_right )
{
    if (start_right)
    {
        glBegin(GL_QUADS);
        glColor4f(top_right->r, top_right->g, top_right->b, top_right->a-(1.0f-gg_dialog_trans()));
        glVertex3f(x + w, y, 0.0f);
        glColor4f(bottom_right->r, bottom_right->g, bottom_right->b, bottom_right->a-(1.0f-gg_dialog_trans()));
        glVertex3f(x + w, y + h, 0.0f);
        glColor4f(bottom_left->r, bottom_left->g, bottom_left->b, bottom_left->a-(1.0f-gg_dialog_trans()));
        glVertex3f(x, y + h, 0.0f);
        glColor4f(top_left->r, top_left->g, top_left->b, top_left->a-(1.0f-gg_dialog_trans()));
        glVertex3f(x, y, 0.0f);
        glEnd();
    }
    else
    {
        glBegin(GL_QUADS);
        glColor4f(top_left->r, top_left->g, top_left->b, top_left->a-(1.0f-gg_dialog_trans()));
        glVertex3f(x, y, 0.0f);
        glColor4f(top_right->r, top_right->g, top_right->b, top_right->a-(1.0f-gg_dialog_trans()));
        glVertex3f(x + w, y, 0.0f);
        glColor4f(bottom_right->r, bottom_right->g, bottom_right->b, bottom_right->a-(1.0f-gg_dialog_trans()));
        glVertex3f(x + w, y + h, 0.0f);
        glColor4f(bottom_left->r, bottom_left->g, bottom_left->b, bottom_left->a-(1.0f-gg_dialog_trans()));
        glVertex3f(x, y + h, 0.0f);
        glEnd();
    }
}

void gui_draw_image(void *image, gg_rect_t source, gg_rect_t dest, int mode_h, int mode_v, gg_colour_t *colour, int effect )
{
    texture_t *texture=image;

    draw_texture(texture, source, dest, mode_h, mode_v, colour, effect );
}

unsigned int gui_get_ticks()
{
    return SDL_GetTicks();
}

void *gui_get_char_image(int c)
{
    return &get_font()->characters[c-32];
}

void gui_get_image_size(void *image, int *width, int *height)
{
    texture_t *temp=image;
   
    if (width)
        *width=temp->width;

    if (height)
        *height=temp->height;
}

void gui_draw_char(int c, int x, int y, gg_colour_t *colour)
{
    /* blank.. */
}

void gui_get_char_size(int c, int *width, int *height)
{
    if (width)
        *width = get_font()->widths[c-32]+1.0f;

    if (height)
        *height = get_font()->height; 
}

void gui_get_string_size(char *s, int *width, int *height)
{
    if (width)
        *width=get_text_len( s );

    if (height)
        *height=get_font()->height;
}

void gui_reset_transition()
{
    gui_transition=FALSE;
}

void gui_draw_dialog_fade()
{
    float flash;

    if (!gui_transition)
    {
        gui_transition=TRUE;
        gui_transition_inc=0.0f;
    }
    else if ( gui_transition_inc < -720 )
    {
        gui_transition_inc=0.0f;
    }
    else
        gui_transition_inc-=0.6;

    glPushMatrix();
    flash=1.0-(-gui_transition_inc/1000);
    glColor4f( flash, flash, flash, 1.0f );

    glPopMatrix();

    glColor4f( 1.0f, 1.0f, 1.0f, 1.0f );

    glPushMatrix();
    glTranslatef( 320.0f, 240.0f, 0.0f );
    glRotatef( gui_transition_inc, 0.0f, 0.0f, 1.0f );
    glScalef( gui_transition_inc/100, gui_transition_inc/100, 1.0f );

    glPopMatrix();
   
    glPushMatrix();
    glTranslatef( 160.0f, 120.0f, 0.0f );
    glRotatef( gui_transition_inc/4, 0.0f, 0.0f, 1.0f );

    glPopMatrix();

    glPushMatrix();
    glTranslatef( 480.0f, 360.0f, 0.0f );
    glRotatef( -gui_transition_inc/2, 0.0f, 0.0f, 1.0f );

    glPopMatrix();
}

gg_driver_t gg_driver_sdlgl =
{
    gui_draw_rect,
    gui_draw_rect_fill,
    gui_draw_image,
    gui_get_char_image,
    gui_draw_char,
    gui_get_image_size,
    gui_get_char_size,
    gui_get_ticks
};

void init_gui()
{
    int i;

    gg_system_init(&gg_driver_sdlgl);
    
    style_menu.textured = 1;
    style_menu.fade_col = gg_colour(1.0f, 0.0f, 0.0f, 1.0f);
    style_menu.hor_pad = 5;
    style_menu.vert_pad = 5;

    load_border( menu_border, "data/menu_border.png");
    load_widget_images( widget_images, "data/widgets.png" );

    for (i = 0; i < 8; i++)
        style_menu.widget_images[i] = &widget_images[i];

    for (i = 0; i < 9; i++)
        style_menu.border.textured.image[i] = &menu_border[i];

    gg_dialog_set_current_style( &style_menu );
}

gg_event_t convert_event(SDL_Event *event)
{
    gg_event_t gg_event;

    gg_event.type=GG_EVENT_NONE;

    switch (event->type)
    {
    case SDL_KEYDOWN:
        gg_event.type = GG_EVENT_KEY;
        switch (event->key.keysym.sym)
        {
        case SDLK_RIGHT:
            gg_event.key = GG_KEY_RIGHT;
            break;
        case SDLK_LEFT:
            gg_event.key = GG_KEY_LEFT;
            break;
        case SDLK_UP:
            gg_event.key = GG_KEY_UP;
            break;
        case SDLK_DOWN:
            gg_event.key = GG_KEY_DOWN;
            break;
        case SDLK_HOME:
            gg_event.key = GG_KEY_HOME;
            break;
        case SDLK_END:
            gg_event.key = GG_KEY_END;
            break;
        case SDLK_RETURN:
            gg_event.key = GG_KEY_ACTION;
            break;
        case SDLK_BACKSPACE:
            gg_event.key = GG_KEY_BACKSPACE;
            break;
        default:
            if (event->key.keysym.unicode <= 0xff)
            {
                gg_event.key = event->key.keysym.unicode;
            }
            else
            {
                gg_event.type = GG_EVENT_NONE;
                return gg_event;
            }
        }
        break;

    case SDL_JOYHATMOTION:
        gg_event.type = GG_EVENT_KEY;
        switch (event->jhat.value)
        {
        case SDL_HAT_RIGHT:
            gg_event.key = GG_KEY_RIGHT;
            break;
        case SDL_HAT_LEFT:
            gg_event.key = GG_KEY_LEFT;
            break;
        case SDL_HAT_UP:
            gg_event.key = GG_KEY_UP;
            break;
        case SDL_HAT_DOWN:
            gg_event.key = GG_KEY_DOWN;
            break;
        default:
            gg_event.type = GG_EVENT_NONE;
            return gg_event;
        }
        break;

    case SDL_JOYBUTTONDOWN:
        gg_event.type = GG_EVENT_KEY;
        switch (event->jbutton.button)
        {
        case 0:
            gg_event.key = GG_KEY_ACTION;
            break;
        case 1:
            gg_event.key = GG_KEY_ESCAPE;
            break;
        case 2:
            gg_event.key = GG_KEY_EXTRA3;
            break;
        case 3:
            gg_event.key = GG_KEY_EXTRA2;
            break;
        case 4:
            gg_event.key = GG_KEY_ESCAPE;
            break;
        default:
            gg_event.type = GG_EVENT_NONE;
            return gg_event;
        }
        break;

    case SDL_MOUSEMOTION:
        gg_event.type = GG_EVENT_MOUSE;
        gg_event.mouse.type = GG_MOUSE_MOVED;
        gg_event.mouse.x = event->button.x;
        gg_event.mouse.y = 479-event->button.y;
        break;

    case SDL_MOUSEBUTTONDOWN:
        gg_event.type = GG_EVENT_MOUSE;
        gg_event.mouse.type = GG_MOUSE_BUTTON_DOWN;
        gg_event.mouse.button = event->button.button - 1;
        gg_event.mouse.x = event->button.x;
        gg_event.mouse.y = 479-event->button.y;
        break;

    case SDL_MOUSEBUTTONUP:
        gg_event.type = GG_EVENT_MOUSE;
        gg_event.mouse.type = GG_MOUSE_BUTTON_UP;
        gg_event.mouse.button = event->button.button - 1;
        gg_event.mouse.x = event->button.x;
        gg_event.mouse.y = 479-event->button.y;
        break;
    }

    return gg_event;
}
