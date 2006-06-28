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
 *  @brief Action widget.
 */

#ifndef GAMEGUI_BUTTON_H
#define GAMEGUI_BUTTON_H

#include <gamegui/system.h>
#include <gamegui/bin.h>

/** Typecast to button widget. */
#define GG_BUTTON(W) GG_CHECK_CAST(W, gg_button_get_class_id(), gg_button_t)

#define GG_BUTTON_DATA \
    GG_BIN_DATA \
    void (* func) (gg_widget_t *widget, void *data); \
    void *func_data;

typedef struct gg_button
{
    GG_BUTTON_DATA
    int pressed;
}
gg_button_t;

gg_class_id gg_button_get_class_id();

void gg_button_render(gg_widget_t *widget, int x, int y, int focus);

int gg_button_input(gg_widget_t *widget, gg_event_t event);

void gg_button_init(gg_button_t *button, gg_widget_t *widget);

gg_widget_t *gg_button_create(char *text);
//gg_widget_t *gg_button_create(gg_widget_t *widget);
//gg_widget_t *gg_button_create_with_label(char *text, float xalign, float yalign);

void gg_button_set_callback(gg_button_t *button, void (* callback) (gg_widget_t *, void *), void *func_data);

#endif /* GAMEGUI_BUTTON_H */
