
#include "main.h"

static gg_colour_t col_black =
{
    0.0f, 0.0f, 0.0f, 1.0f
};

static gg_colour_t col_white =
{
    1.0f, 1.0f, 1.0f, 1.0f
};

/* Here goes our drawing code */
void draw_scene()
{
    int size;
    gg_rect_t area;

    gg_dialog_cleanup();

    /*gg_system_get_image_size(get_menu_style()->border.textured.image[0], &size, NULL);
    area.x = 0; area.y = 0;
    gui_get_string_size(get_chat_buffer(), &area.width, &area.height);*/

    /* Clear The Screen And The Depth Buffer */
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glLoadIdentity();

    if ((!gg_dialog_current()) || (gg_dialog_current() && !gg_dialog_current()->modal))
    {
        // not in dialog.. reset transition state..
        gui_reset_transition();
        
        glPushMatrix();
        glTranslatef( -get_camera_x(), -get_camera_y(), 0.0f );
        glColor3f( 1.0f, 1.0f, 1.0f );
        draw_map();
        glPopMatrix();
    }

    if (gg_dialog_current())
    {
        if  (gg_dialog_current()->modal)
        {
            glPushMatrix();
            gui_draw_dialog_fade();
            glPopMatrix();
        }

        glPushMatrix();
        gg_dialog_t *dialog = gg_dialog_current();
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        gg_dialog_render(dialog);
        glDisable(GL_BLEND);
        glPopMatrix();
    }
 
  /*  if ( strlen(get_chat_buffer()) > 0 )
    {
        glPushMatrix();
        glTranslatef( 20.0f, 20.0f, 0.0f );
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);     
        draw_border(get_menu_style()->border.textured.image, area, size);    
        gg_system_draw_string(get_chat_buffer(), 0, 0, &col_white, 0, 0, 0);
        glDisable(GL_BLEND);
        glPopMatrix();
    }*/

    glPushMatrix();
    glTranslatef(get_mouse_x(), get_mouse_y(), 0.0f );
    glColor3f( 1.0f, 1.0f, 1.0f );
    draw_mouse_cursor();
    glPopMatrix();

    if ( glGetError() != GL_NO_ERROR )
        printf( "We've go an error.\n" );

    /* Draw it to the screen */
    SDL_GL_SwapBuffers();
}
