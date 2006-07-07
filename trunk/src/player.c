
#include "main.h"

int num_players=0;
player_t players[16];
int show_player_list=FALSE;

texture_t player;

int get_local_player_index()
{
    int retval;

    if ( get_server_active() )
        retval=0;
    else
        retval=get_client_slot();

    return retval;
}

void move_player( int index, float xinc, float yinc )
{
    players[index].xpos+=xinc;
    players[index].ypos+=yinc;
}

void load_player_tex()
{
    load_texture( &player, "./data/player_temp.png", TRUE );
}

static gg_colour_t col_white =
{
    1.0f, 1.0f, 1.0f, 1.0f
};

void draw_player_list()
{
    char temp_str[80];
    gg_rect_t area;
    int size,i=0;
    int text_pos=0;

    gg_system_get_image_size(get_menu_style()->border.textured.image[0], &size, NULL);
    area.x = 30; area.y = 30;
    area.width=200.0f;
    area.height=100.0f;

    /* Draw the layer widget */
    glPushMatrix();
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 

    draw_border(get_menu_style()->border.textured.image, area, size); 

    text_pos=area.y+1;
    if (get_server_active())
        gg_system_draw_string("Acting as Server", area.x+5, text_pos, &col_white, 0, 0, 0);
    else if (get_client_active())
        gg_system_draw_string("Acting as Client", area.x+5, text_pos, &col_white, 0, 0, 0);
    else
        gg_system_draw_string("Not active.", area.x+5, text_pos, &col_white, 0, 0, 0);
    text_pos+=17;

    gg_system_draw_string("Players Connected:", area.x+5, text_pos, &col_white, 0, 0, 0);
    text_pos+=17;

    for (i=0;i<MAX_PLAYERS;i++)
    {
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

int get_show_player_list()
{
    return show_player_list;
}

void set_show_player_list( int show )
{
    show_player_list=show;
}

player_t *get_player( int index )
{
    return &players[index];
}

void set_num_players( int count )
{
    num_players=count;
}

int get_num_players()
{
    return num_players;
}

void set_player_active( int index, int active )
{
    players[index].active=active;
}

int get_player_active( int index )
{
    return players[index].active;
}

void set_player_name( int index, char *name )
{
    strcpy(players[index].name,name);
}

char *get_player_name( int index )
{
    return players[index].name;
}

void draw_player( int index )
{
    glEnable( GL_TEXTURE_2D );
    glBindTexture( GL_TEXTURE_2D, player.gl_index );

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glTranslatef( players[index].xpos, players[index].ypos, 0.0f );

    glBegin( GL_QUADS ); 
        glTexCoord2f( 0.0f, 0.0f ); /* Top Left */
        glVertex3f( 0.0f,  0.0f, 0.0f );

        glTexCoord2f( 1.0f, 0.0f ); /* Top Right */
        glVertex3f( 128, 0.0f, 0.0f );

        glTexCoord2f( 1.0f, 1.0f ); /* Bottom Right */
        glVertex3f( 128, 128,  0.0f );

        glTexCoord2f( 0.0f, 1.0f ); /* Bottom Left */
        glVertex3f( 0.0f,  128,  0.0f );
    glEnd( ); 

    glDisable( GL_BLEND );
    glDisable( GL_TEXTURE_2D );
}

void draw_players()
{
    int i=0;

    for (i=0;i<MAX_PLAYERS;i++)
    {
        if ( players[i].active )
        {
            glPushMatrix();
            // draw face..
            draw_player( i );
            // draw name..
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            gg_system_draw_string(players[i].name, 0, 0, &col_white, 0, 0, 0);
            glDisable( GL_BLEND );
            glPopMatrix();
        }
    }
}

