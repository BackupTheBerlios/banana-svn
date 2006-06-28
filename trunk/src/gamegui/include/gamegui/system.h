/*  DreamChess
 *  Copyright (C) 2005  The DreamChess project
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/** @file
 *  @brief Core gamegui system header file.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifndef GG_SYSTEM_H
#define GG_SYSTEM_H

#define TRUE 1
#define FALSE 0

/** No children should get focus. */
#define GG_FOCUS_NONE 0
/** At most one child should get focus. */
#define GG_FOCUS_ONE 1
/** All children should get focus. */
#define GG_FOCUS_ALL 2

/** Bouncy text amplitude in pixels. */
#define GG_BOUNCE_AMP 2
/** Bouncy text wave length in characters. */
#define GG_BOUNCE_LEN 10
/** Bouncy text speed in bounces per second. */
#define GG_BOUNCE_SPEED 3

/** Unique value used to identify a class. */
typedef int gg_class_id;

/** Colour description. */
typedef struct gg_colour
{
    /** Red channel. Ranges from 0.0f to 1.0f. */
    float r;

    /** Green channel. Ranges from 0.0f to 1.0f. */
    float g;

    /** Blue channel. Ranges from 0.0f to 1.0f. */
    float b;

    /** Alpha channel. Ranges from 0.0f (transparent) to 1.0f (opaque). */
    float a;
}
gg_colour_t;

/** Rectangle. */
typedef struct gg_rect
{
    /** Leftmost x coordinate of the rectangle. */
    int x;
    /** Lowermost y coordinate of the rectangle. */
    int y;
    /** Width of the rectangle in pixels. */
    int width;
    /** Height of the rectangle in pixels. */
    int height;
}
gg_rect_t;

/** Tile image to fill target area. */
#define GG_MODE_TILE 0
/** Scale image to fill target area. */
#define GG_MODE_SCALE 1

/** Gamegui driver. Gamegui uses these functions to do all its rendering. */
typedef struct gg_driver
{
    void (* draw_rect) (int x, int y, int width, int height, gg_colour_t *colour);
    void (* draw_filled_rect) (int x, int y, int width, int height, 
        gg_colour_t *top_left, gg_colour_t *top_right, gg_colour_t *bottom_left, gg_colour_t *bottom_right, int start_right );

    void (* draw_image) (void *image, gg_rect_t source, gg_rect_t dest, int mode_h, int mode_v, gg_colour_t *colour, int glow);
    void *(* get_char_image) (int c);
    void (* draw_char) (int c, int x, int y, gg_colour_t *colour);
    void (* get_image_size) (void *image, int *width, int *height);
    void (* get_char_size) (int c, int *width, int *height);
    unsigned int (* get_ticks) ();
}
gg_driver_t;

/** Value indicating an unregistered class. */
#define GG_CLASS_ID_NONE -1

/** Prints a typecasting error message and aborts the program. */
#define GG_CAST_ERROR(C) ((C *) gg_cast_error(__FILE__, __LINE__, #C))

#define GG_CHECK_CAST(W, I, C) ((gg_check_cast((gg_widget_t *) W, I)) ? (C *) W \
    : GG_CAST_ERROR(C))

#define GG_CHILD(C) \
    static gg_class_id class_id = GG_CLASS_ID_NONE;\
    if (class_id == GG_CLASS_ID_NONE) \
        class_id = gg_register_class(C); \
    return class_id;

struct gg_widget;

struct gg_widget *gg_cast_error(char *file, int line, char *type);

gg_class_id gg_register_class(gg_class_id parent);

int gg_check_cast(struct gg_widget *widget, gg_class_id id);

void gg_system_init(gg_driver_t *d);

void gg_system_draw_rect(int x, int y, int width, int height, gg_colour_t *colour);

void gg_system_draw_filled_rect(int x, int y, int width, int height, gg_colour_t *colour);

void gg_system_draw_gradient_rect(int x, int y, int width, int height, 
    gg_colour_t *top_left, gg_colour_t *top_right, gg_colour_t *bottom_left, gg_colour_t *bottom_right, int start_right );

void gg_system_draw_image(void *image, gg_rect_t source, gg_rect_t dest, int mode_h, int mode_v, gg_colour_t *colour, int glow);

void gg_system_draw_char(int c, int x, int y, gg_colour_t *colour);

void gg_system_get_image_size(void *image, int *width, int *height);

void gg_system_get_char_size(int c, int *width, int *height);

void gg_system_get_string_size(char *s, int *width, int *height);

void gg_system_draw_string(char *s, int x, int y, gg_colour_t *colour, int bounce, int effect, float align);

unsigned int gg_system_get_ticks();

gg_colour_t gg_colour(float r, float g, float b, float a);

gg_rect_t gg_rect(int x, int y, int w, int h);

#endif /* GG_SYSTEM_H */
