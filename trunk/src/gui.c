
#include "main.h"

gg_dialog_style_t style_menu;
texture_t menu_border[9];

int gui_transition=FALSE;
float gui_transition_inc=0.0f;

void draw_glow( int x, int y, int width, int height, int glow_width, gg_colour_t *col, int middle )
{
    gg_colour_t solid =
    {
        col->r, col->g, col->b, 1.0f
    };

    gg_colour_t trans =
    {
        col->r, col->g, col->b, 0.0f
    };

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
    glColor4f(col->r, col->g, col->b, col->a);
    glBegin(GL_LINE_LOOP);
    glVertex3f(x + 0.5f, y + 0.5f, 1.0f);
    glVertex3f(x + w + 0.5f, y + 0.5f, 1.0f);
    glVertex3f(x + w + 0.5f, y + h + 0.5f, 1.0f);
    glVertex3f(x + 0.5f, y + h + 0.5f, 1.0f);
    glEnd();
}

void gui_draw_rect_fill(int x, int y, int w, int h, 
    gg_colour_t *top_left, gg_colour_t *top_right, gg_colour_t *bottom_left, gg_colour_t *bottom_right, int start_right )
{
    if (start_right)
    {
        glBegin(GL_QUADS);
        glColor4f(top_right->r, top_right->g, top_right->b, top_right->a);
        glVertex3f(x + w, y, 0.0f);
        glColor4f(bottom_right->r, bottom_right->g, bottom_right->b, bottom_right->a);
        glVertex3f(x + w, y + h, 0.0f);
        glColor4f(bottom_left->r, bottom_left->g, bottom_left->b, bottom_left->a);
        glVertex3f(x, y + h, 0.0f);
        glColor4f(top_left->r, top_left->g, top_left->b, top_left->a);
        glVertex3f(x, y, 0.0f);
        glEnd();
    }
    else
    {
        glBegin(GL_QUADS);
        glColor4f(top_left->r, top_left->g, top_left->b, top_left->a);
        glVertex3f(x, y, 0.0f);
        glColor4f(top_right->r, top_right->g, top_right->b, top_right->a);
        glVertex3f(x + w, y, 0.0f);
        glColor4f(bottom_right->r, bottom_right->g, bottom_right->b, bottom_right->a);
        glVertex3f(x + w, y + h, 0.0f);
        glColor4f(bottom_left->r, bottom_left->g, bottom_left->b, bottom_left->a);
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
   // printf( "%i\n", c);
    return &get_big_font()->characters[c-32];
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
    //text_draw_char(x, y, 1.0f, c, colour);
}

void gui_get_char_size(int c, int *width, int *height)
{
    if (width)
        *width = get_big_font()->widths[c-32]+1.0f;

    if (height)
        *height = get_big_font()->size; 
}

void gui_get_string_size(char *s, int *width, int *height)
{
    if (width)
        *width=get_text_len( get_big_font(), s );

    if (height)
        *height=get_text_size( get_big_font() );
}

void gui_get_text_spacing( int *spacing )
{
    *spacing=1.0f;
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
 
    //printf( "inc: %f\n", gui_transition_inc );

    glPushMatrix();
    flash=1.0-(-gui_transition_inc/1000);
    glColor4f( flash, flash, flash, 1.0f );
    //draw_texture( get_framebuffer_grab() );
    glPopMatrix();

    glColor4f( 1.0f, 1.0f, 1.0f, 1.0f );

    glPushMatrix();
    glTranslatef( 320.0f, 240.0f, 0.0f );
    glRotatef( gui_transition_inc, 0.0f, 0.0f, 1.0f );
    glScalef( gui_transition_inc/100, gui_transition_inc/100, 1.0f );
    //draw_texture( get_framebuffer_grab() );
    glPopMatrix();
   
    glPushMatrix();
    glTranslatef( 160.0f, 120.0f, 0.0f );
    glRotatef( gui_transition_inc/4, 0.0f, 0.0f, 1.0f );
    //draw_texture( get_framebuffer_grab() );
    glPopMatrix();

    glPushMatrix();
    glTranslatef( 480.0f, 360.0f, 0.0f );
    glRotatef( -gui_transition_inc/2, 0.0f, 0.0f, 1.0f );
    //draw_texture( get_framebuffer_grab() );
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
    gui_get_ticks,
    gui_get_text_spacing
};

void init_gui()
{
    int i;

    gg_system_init(&gg_driver_sdlgl);
    
    style_menu.textured = 1;
    style_menu.fade_col = gg_colour(1.0f, 0.0f, 0.0f, 1.0f);
    style_menu.hor_pad = 20;
    style_menu.vert_pad = 10;

    load_border( menu_border, "data/menu_border.png");

    for (i = 0; i < 9; i++)
        style_menu.border.textured.image[i] = &menu_border[i];
}

gg_event_t convert_event(SDL_Event *event)
{
    static unsigned int pressed;
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

    case SDL_MOUSEBUTTONDOWN:
        gg_event.type = GG_EVENT_MOUSE;
        gg_event.mouse.type = GG_MOUSE_BUTTON_DOWN;
        gg_event.mouse.button = event->button.button - 1;
        gg_event.mouse.x = event->button.x;
        gg_event.mouse.y = event->button.y;
        break;
    }

    return gg_event;
}
