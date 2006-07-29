
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

static gg_colour_t col_red =
{
    1.0f, 0.0f, 0.0f, 1.0f
};

static int fps_enabled = 0;
static int frames = 0;
static Uint32 fps_time = 0;
static float fps;

float get_fps()
{
    return fps;
}

/* Render the scene */
void render_scene()
{
    gg_dialog_t *dialog;
    static Uint32 last = 0;
    Uint32 now;
    char temp[80];
    int i;

    fps_enabled=TRUE;

    /* Clean up any GameGUI dialogs */
    gg_dialog_cleanup();

    /* Clear the screen and depth buffer */
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glLoadIdentity();

    /* Draw the map */
    glPushMatrix();
     glColor3f( 1.0f, 1.0f, 1.0f );
     draw_map();
    glPopMatrix();

    /* Draw players */
    glPushMatrix();
     draw_players();
    glPopMatrix();

    /* If GameGUI dialogs are active, draw them. */
    for (i=0;i<gg_dialog_count();i++)
    {
        glPushMatrix();
         dialog=gg_get_dialog(i);
         glEnable(GL_BLEND);
         glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
         /*if (i==gg_dialog_count()-1)*/
            gg_dialog_set_trans( 1.0f );
         /*else   
            gg_dialog_set_trans( 0.5f );*/
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

    /* Draw the FPS if enabled */
    if (fps_enabled)
    {
        glPushMatrix();
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
        sprintf(temp, "FPS: %.2f", fps);
        gg_system_draw_string(temp, 5, 5, &col_red, 0, 0, 0);
        /*glTranslatef(0.0f,17.0f,0.0f);
        sprintf(temp, "Net In: %i", *get_pps_in());
        gg_system_draw_string(temp, 5, 5, &col_red, 0, 0, 0);
        glTranslatef(0.0f,17.0f,0.0f);
        sprintf(temp, "Net Out: %i", *get_pps_out());
        gg_system_draw_string(temp, 5, 5, &col_red, 0, 0, 0);*/
        glDisable(GL_BLEND);
        glPopMatrix();
    }

    /* temp */
    /*glPushMatrix();
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
    glTranslatef(0.0f,17.0f*4,0.0f);
    sprintf(temp, "Animation frames per second: %.2f", get_player(get_local_player_index())->current_anim->speed);
    gg_system_draw_string(temp, 5, 5, &col_red, 0, 0, 0);
    glDisable(GL_BLEND);
    glPopMatrix();*/
    /* temp */

    /* Swap the buffers. */
    SDL_GL_SwapBuffers();

    /* FPS stuff.. */
    now = SDL_GetTicks();
    if (now - last < 1000 / FPS)
        SDL_Delay(1000 / FPS - (now - last));
    last = SDL_GetTicks();

    frames++;
    if (frames == 10)
    {
        fps = 10000 / (float) (last - fps_time);
        frames = 0;
        fps_time = last;
    }
}
