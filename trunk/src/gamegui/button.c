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

#include <stdlib.h>

#include <gamegui/button.h>
#include <gamegui/label.h>

gg_colour_t col_blue =
{
    0.0f, 1.0f, 1.0f, 1.0f
};

gg_colour_t col_white =
{
    1.0f, 1.0f, 1.0f, 1.0f
};

gg_colour_t col_black =
{
    0.0f, 0.0f, 0.0f, 1.0f
};

int button_tspacing=4;

gg_class_id gg_button_get_class_id()
{
    GG_CHILD(gg_bin_get_class_id())
}

/** Implements widget::render for button widgets. */
void gg_button_render(gg_widget_t *widget, int x, int y, int focus)
{
    gg_button_t *button = GG_BUTTON(widget);
    gg_widget_t *child = gg_bin_get_child(GG_BIN(widget));
    gg_label_t *label = GG_LABEL(child);
    int glow_width=5;

    int width;
    gg_system_get_string_size(label->label, &width, NULL);

    if (focus != GG_FOCUS_NONE)
        focus = GG_FOCUS_ALL;

    if (focus)
    {
        draw_glow( x+16, y+6, width, label->height_a-10, glow_width, &col_blue, TRUE );
        gg_system_draw_string(label->label, x+16, y+5, &col_white, 0, 1, 0);
    }
    else
    {
        gg_system_draw_string(label->label, x+16, y+6, &col_white, 0, 1, 0);
    }
}

/** Implements widget::input for button widgets. */
int gg_button_input(gg_widget_t *widget, gg_event_t event)
{
    gg_button_t *button = GG_BUTTON(widget);

    if ((event.type == GG_EVENT_KEY && event.key == GG_KEY_ACTION)
            || (event.type == GG_EVENT_MOUSE &&
                event.mouse.type == GG_MOUSE_BUTTON_DOWN &&
                event.mouse.button == 0))
    {

        if (button->func)
            button->func(widget, button->func_data);

        return 1;
    }

    return 0;
}

void gg_button_init(gg_button_t *button, gg_widget_t *widget)
{
    gg_bin_init((gg_bin_t *) button, widget);

    button->render = gg_button_render;
    button->input = gg_button_input;
    button->id = gg_button_get_class_id();
    button->func = NULL;
    button->func_data = NULL;
    button->enabled = 1;
    button->width = widget->width+32; /* FIXME */
    button->height = widget->height+5; /* FIXME */
}

/** @brief Creates an button widget.
 *
 *  An button widget consists of a single label. When the widget is activated
 *  a function is executed.
 *
 *  @param string The text for the widget.
 *  @return The created button widget.
 */
/*gg_widget_t *gg_button_create(gg_widget_t *widget)
{
    gg_button_t *button = malloc(sizeof(gg_button_t));

    gg_button_init(button, widget);

    return GG_WIDGET(button);
}*/

gg_widget_t *gg_button_create(char *text)
{
    float xalign=0.0f, yalign=0.0f;
    gg_widget_t *label = gg_label_create(text);
    gg_widget_t *button;

    gg_align_set_alignment(GG_ALIGN(label), xalign, yalign);

    button = malloc(sizeof(gg_button_t));
    gg_button_init((gg_button_t*)button, label);

    return button;
}

/** @brief Sets button widget callback.
 *
 *  @param widget The button widget.
 *  @param callback Function that should be called when widget is activated.
 */
void gg_button_set_callback(gg_button_t *button, void (* callback) (gg_widget_t *, void *), void *func_data)
{
    button->func = callback;
    button->func_data = func_data;
}
