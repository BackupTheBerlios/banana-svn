
/*
    Banana - A multiplayer platform game engine.
    Copyright(C) 2006, Kris McAulay  

    scene.c - Render the scene.
*/

#include "main.h"

static gg_colour_t col_white =
{
    1.0f, 1.0f, 1.0f, 1.0f
};

/* Render the scene */
void render_scene()
{
    gg_dialog_t *dialog;
    int i;

    /* Clean up any GameGUI dialogs */
    gg_dialog_cleanup();

    /* Clear the screen and depth buffer */
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glLoadIdentity();

    /* Draw the map */
    glPushMatrix();
     glTranslatef( -get_camera_x(), -get_camera_y(), 0.0f );
     glColor3f( 1.0f, 1.0f, 1.0f );
     draw_map();
    glPopMatrix();

    /* Draw players */
    glPushMatrix();
     draw_players();
    glPopMatrix();

    /* If a GameGUI dialog is active, draw it. */
    if (gg_dialog_current())
    {
        glPushMatrix();
         dialog = gg_dialog_current();
         glEnable(GL_BLEND);
         glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
         gg_dialog_render(dialog);
         glDisable(GL_BLEND);
        glPopMatrix();
    }
 
    /* Draw the chat buffer. */
    glPushMatrix();
     glTranslatef( 20.0f, 20.0f, 0.0f );
     for (i=0;i<5;i++)
     {
         glEnable(GL_BLEND);
         glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
         gg_system_draw_string(get_chat_buffer_line(i), 0, 0, &col_white, 0, 0, 0);
         glDisable(GL_BLEND);
         glTranslatef( 0.0f, 20.0f, 0.0f );
     }
    glPopMatrix();
   
    /* Draw player list */
    if (get_show_player_list())
        draw_player_list();

    /* If editing, draw edit mode widgets */
    if ( get_editing() )
        draw_edit_widgets(); 

    /* Draw mouse cursor */
    if (gg_dialog_current() || get_editing() )
    {
        glPushMatrix();
         glTranslatef(get_mouse_x(), get_mouse_y(), 0.0f );
         glColor3f( 1.0f, 1.0f, 1.0f );
         draw_mouse_cursor();
        glPopMatrix();
    }

    /* Swap the buffers. */
    SDL_GL_SwapBuffers();
}
