
#ifndef __MAIN_H
#define __MAIN_H

#include <stdlib.h>
#include <string.h>
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_net.h"
#include "SDL_thread.h"
#include "SDL_opengl.h"
#include "gamegui.h"

/* screen width, height, and bit depth */
#define SCREEN_WIDTH  640
#define SCREEN_HEIGHT 480
#define SCREEN_BPP     32

#define MAX_PLAYERS    16

/* Define our booleans */
#define TRUE  1
#define FALSE 0

/* Dialog idents.. probably no longer needed.. */
#define DIALOG_SAVEMAP   0
#define DIALOG_EDITTOOL  0

/* Widgets */
#define WIDGET_TEXT_INPUT   0
#define WIDGET_LABEL        1
#define WIDGET_LIST         2
#define WIDGET_BUTTON       3

/* structs */
typedef struct texture
{
    /** OpenGL Texture ID. */
    GLuint gl_index;

    /** Upper-left u-coordinate. Ranges from 0.0f to 1.0f. */
    float u1;

    /** Upper-left v-coordinate. Ranges from 0.0f to 1.0f. */
    float v1;

    /** Lower-right u-coordinate. Ranges from 0.0f to 1.0f. */
    float u2;

    /** Lower-right v-coordinate. Ranges from 0.0f to 1.0f. */
    float v2;

    /** Width of texture in pixels. */
    int width;

    /** Height of texture in pixels. */
    int height;
}
texture_t;

typedef struct font
{
    texture_t characters[96];
    char widths[96];
    int height;

}font_t;

typedef struct player
{
    float xpos, ypos;
    char name[80];
    int active;

    IPaddress *ip;
    TCPsocket socket;
}player_t;

/* main.c */
int get_videoflags();
void quit( int returnCode );

/* init.c */
void init_video();
void init_net();

/* gameloop.c */
void gameloop();

/* scene.c */
void render_scene();

/* init.c */
void resize_window( int width, int height );

/* texture.c */
#define NO_ALPHA    0
#define USE_ALPHA   1
void load_texture( texture_t *texture, char *filename, int alpha );
texture_t SDL_GL_LoadTexture(SDL_Surface *surface, SDL_Rect *area, int alpha);
void grab_framebuffer();
texture_t *get_framebuffer_grab();
void draw_texture(texture_t *texture, gg_rect_t source, gg_rect_t dest, int mode_h, int mode_v, gg_colour_t *colour, int effect );

/* tilebay.c */
#define TILE_SIZE   32
texture_t *get_tilebay();
void load_tilebay( char *filename );
void draw_tile( int index );

/* map.c */
typedef struct layer
{
    int width, height;
    char *tiles;

}layer;

void set_tile( layer *lay, int xpos, int ypos, int index );
int get_tile( layer *lay, int xpos, int ypos );
void load_map( char *filename );
layer *get_player_layer();
layer *get_obs_layer();
layer *create_layer( int width, int height );

/* draw_map.c */
void draw_layer( layer *lay );
void draw_map();

/* mouse.c */
void draw_mouse_cursor();
void load_mouse_cursor( char *filename );
int get_mouse_x();
int get_mouse_y();
void process_mouse();

/* camera.c */
void set_camera( float xpos, float ypos );
float get_camera_x();
float get_camera_y();

/* keyboard.c */
void process_keyboard();

/* save_dialog.c */
void create_save_dialog();
void open_save_dialog();
void process_save_dialog();

/* text.c */
void load_font( char *texfile, char *widfile );
font_t *get_font();

/* gui.c */
void init_gui();
gg_dialog_style_t *get_menu_style();
gg_event_t convert_event(SDL_Event *event);
void draw_glow( int x, int y, int height, int width, int glow_width, gg_colour_t *col, int middle );
void gui_get_char_size(int c, int *width, int *height);
void *gui_get_char_image(int c);
void gui_get_string_size(char *s, int *width, int *height);

/* net_server.c */
#define MESG_CHAT            'A'     /* Send a line of text for the char buffer.*/
#define MESG_CONNECT         'B'     /* A client is asking to connect. */
#define MESG_DISCONNECT      'C'     /* A client wants to disconnect. */
#define MESG_NEWPLAYER       'D'     /* Notify clients of a new player. */
#define MESG_PLAYERLEAVE     'E'     /* Notify clients of a disconnected player. */
#define MESG_PLAYERMOVED     'F'     /* Tell a server/client that a player has moved */
#define MESG_WHICHSLOT       'G'     /* Tell the client which slot it is */

/* v--- Junk messages. */
#define MESG_TILE       'Z'     /* Tell server/client to change a tile at 1+2 to 1*/

void start_server( int port, char *nickname );
void server_listen();
int get_server_active();
TCPsocket get_client_socket( int index );

/* net_client.c */
void client_listen();
int get_client_active();
void connect_to_server( char *host, int port, char *nickname );
TCPsocket get_server_socket();
int get_client_slot();
void send_disconnect_message();

/* net_common.c */
#define CHAT_LINES 5
void send_message( char *buffer, int len );
void send_message_omit( int omited_index, char *buffer, int len );
void net_change_tile( int x, int y );
char *get_chat_buffer_line( int line );
void add_chat_buffer_line( char *text );
int get_chat_buffer_lines();
void send_chat_message( char *text );
void send_who_message( TCPsocket socket, char *text );
void send_playermoved_message( int index, int xpos, int ypos );

/* player.c */
player_t *get_player( int index );
void set_num_players( int count );
int get_num_players();
void set_player_active( int index, int active );
int get_player_active( int index );
void set_player_name( int index, char *name );
char *get_player_name( int index );
void draw_players();
void toggle_player_list();
int get_show_player_list();
void draw_player_list();
void load_player_tex();
void move_player( int index, float xinc, float yinc );
int get_local_player_index();
void set_show_player_list( int show );

/* editmode.c */
#define EDITING_PLAYER_LAYER    0
#define EDITING_OBS_LAYER       1
int get_editmode();
int get_editing();
void set_editing( int edit );
void draw_edit_widgets();

/* Dialogs. */
void show_title_dialog( int modal );
void show_server_dialog( int modal );
void show_client_dialog( int modal );
void show_chat_dialog( int modal );

#endif /* __MAIN_H */
