
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
#define TILE_SIZE      32

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
    int size;

}font_t;

typedef struct player
{
    float xpos, ypos;
    char name[80];

    IPaddress *ip;
    TCPsocket socket;
}player_t;

/* main.c */
int get_videoflags();
void quit( int returnCode );

/* draw_tile.c */
void draw_tile( int index );

/* init.c */
void init_video();
void init_net();

/* gameloop.c */
void gameloop();

/* scene.c */
void draw_scene();

/* init.c */
void resize_window( int width, int height );

/* texture.c */
void load_texture( texture_t *texture, char *filename, int alpha );
texture_t SDL_GL_LoadTexture(SDL_Surface *surface, SDL_Rect *area, int alpha);
void grab_framebuffer();
texture_t *get_framebuffer_grab();
//void draw_texture( texture_t *texture );
void draw_texture(texture_t *texture, gg_rect_t source, gg_rect_t dest, int mode_h, int mode_v, gg_colour_t *colour, int effect );

/* tilebay.c */
texture_t *get_tilebay();
void load_tilebay( char *filename );

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

/* edittools_dialog.c */
void create_edittool_dialog();
void open_edittool_dialog();
void process_edittool_dialog();

/* text.c */
font_t *load_font( char *texfile, char *widfile );
//void draw_string( font_t *f, char *text );
void draw_string(font_t *font, char *s, int x, int y, gg_colour_t *colour, int bounce, int effect, float align);
int draw_partial_string( font_t *f, char *text, int xpos, int len );
void draw_char( font_t *f, char c );
int get_text_len( font_t *f, char *string );
int get_text_size( font_t *f );
void load_fonts();
font_t *get_big_font();
font_t *get_small_font();

/* gui.c */
void init_gui();
gg_dialog_style_t *get_menu_style();
gg_event_t convert_event(SDL_Event *event);
void draw_glow( int x, int y, int height, int width, int glow_width, gg_colour_t *col, int middle );
void gui_get_char_size(int c, int *width, int *height);
void *gui_get_char_image(int c);
void gui_get_string_size(char *s, int *width, int *height);

/* net_server.c */
#define MESG_CHAT   'A'
#define MESG_TILE   'B'
void start_server( int port );
void server_listen();
int get_server_active();

/* net_client.c */
void client_listen();
int get_client_active();
void connect_to_server( char *host, int port );
TCPsocket get_server_socket();

/* net_common.c */
void send_message( char *buffer, int len );
void net_change_tile( int x, int y );

/* player.c */
player_t *get_player( int index );
void set_num_players( int count );
int get_num_players();

/* Dialogs. */
void show_title_dialog( int modal );
void show_server_dialog( int modal );
void show_client_dialog( int modal );

#endif /* __MAIN_H */
