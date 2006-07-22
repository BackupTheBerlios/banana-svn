
/*
    Banana - A multiplayer platform game engine.
    Copyright(C) 2006, Kris McAulay  

    player.c - Manage and draw the players.
*/

#include "main.h"

int num_players=0;
player_t players[16];
int show_player_list=FALSE;

texture_t player;

static gg_colour_t col_white =
{
    1.0f, 1.0f, 1.0f, 1.0f
};

/* Play anims..*/
anim_t walking;

void init_player_animations()
{
    int i=0;

    walking.tex=&player;
    walking.speed=64;
    walking.frame_count=16;
    walking.loop_frame=0;
    walking.width=44;
    walking.height=50;

    walking.frames=malloc(sizeof(frame_pos_t)*16);
    walking.uwidth=(float)walking.width/1024;
    walking.vheight=(float)walking.height/64;

    for ( i=15; i>=0; i-- )
    {
        walking.frames[i].upos=(15-i)*walking.uwidth;
        walking.frames[i].vpos=0.0f;
    }

    /* temp */
    for (i=0; i<MAX_PLAYERS; i++ )
    {
        players[i].current_anim=&walking;
        players[i].anim_pos=0;
        players[i].last_anim_tick=SDL_GetTicks();
    }
    /* temp */
}

/* Get the slot in which the local player resides */
int get_local_player_index()
{
    int retval;

    /* If running server, always 0 */
    if ( get_server_active() )
        retval=0;
    /* If client, get the slot */
    else
        retval=get_client_slot();

    return retval;
}

/* Move player in the specified slot by the specified increments */
void move_player( int index, float xinc, float yinc )
{
    players[index].xpos+=xinc;
    players[index].ypos+=yinc;
}

/* Load the player texture. TEMPORARY */
void load_player_tex()
{
    load_texture( &player, "./data/player_temp2.png", TRUE );
}
/* TEMPORARY */

/* Draw the list of players */
void draw_player_list()
{
    char temp_str[80];
    gg_rect_t area;
    int size,i=0;
    int text_pos=0;

    /* Set up size/position of list, expanding for the player count */
    area.x = 30; area.y = 30;
    area.width=200.0f;
    area.height=35.0f+(get_num_players()*17);

    glPushMatrix();
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 

    /* Fetch the border width, and draw the border */
    gg_system_get_image_size(get_menu_style()->border.textured.image[0], &size, NULL);
    draw_border(get_menu_style()->border.textured.image, area, size); 

    /* Increment text y positiom, and draw a string saying if we are server, or client */
    text_pos=area.y+1;
    if (get_server_active())
        gg_system_draw_string("Acting as Server", area.x+5, text_pos, &col_white, 0, 0, 0);
    else if (get_client_active())
        gg_system_draw_string("Acting as Client", area.x+5, text_pos, &col_white, 0, 0, 0);
    else
        gg_system_draw_string("Not active.", area.x+5, text_pos, &col_white, 0, 0, 0);
    /* Increment text y position */
    text_pos+=17;

    /* Draw the list of players */
    gg_system_draw_string("Players Connected:", area.x+5, text_pos, &col_white, 0, 0, 0);
    text_pos+=17;

    for (i=0;i<MAX_PLAYERS;i++)
    {
        /* If the player is active, add draw the players name and slot, increment text y position */    
        if (get_player_active(i))
        {
            sprintf( temp_str, "(%i): %s", i, get_player_name(i) );
            gg_system_draw_string(temp_str, area.x+15, text_pos, &col_white, 0, 0, 0);
            text_pos+=17;
        }
    }

    glDisable(GL_BLEND);
    glPopMatrix();

}

/* Return whether we draw the player list or not */
int get_show_player_list()
{
    return show_player_list;
}

/* Set whether we draw the player list or not */
void set_show_player_list( int show )
{
    show_player_list=show;
}

/* Return a pointer ot the indexed player */
player_t *get_player( int index )
{
    return &players[index];
}

/* Set the number of players */
void set_num_players( int count )
{
    num_players=count;
}

/* Get the number of players */
int get_num_players()
{
    return num_players;
}

/* Set if a player is active or not */
void set_player_active( int index, int active )
{
    players[index].active=active;
}

/* Get if a player is active or not */
int get_player_active( int index )
{
    return players[index].active;
}

/* Set a player's name */
void set_player_name( int index, char *name )
{
    strcpy(players[index].name,name);
}

/* Get a player's name */
char *get_player_name( int index )
{
    return players[index].name;
}

/* Draw a player */
void draw_player( int index )
{
    draw_anim( players[index].current_anim, 
        players[index].anim_pos, players[index].xpos, players[index].ypos, players[index].facing );
}

/* Draw all players */
void draw_players()
{
    int i=0;

    for (i=0;i<MAX_PLAYERS;i++)
    {
        if ( players[i].active )
        {
            glPushMatrix();

            draw_player(i);

            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            gg_system_draw_string(players[i].name, 0, 0, &col_white, 0, 0, 0);
            glDisable( GL_BLEND );
            glPopMatrix();

            /* Update the player animation.. */
            if ( SDL_GetTicks()-players[i].last_anim_tick > (1000/players[i].current_anim->speed) )
            {
                if ( players[i].anim_pos < players[i].current_anim->frame_count-1 )
                    players[i].anim_pos++;
                else
                    players[i].anim_pos=players[i].current_anim->loop_frame;

                players[i].last_anim_tick=SDL_GetTicks();
            }
        }
    }
}

