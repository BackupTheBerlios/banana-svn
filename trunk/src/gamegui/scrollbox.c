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

#include <gamegui/scrollbox.h>
#include <gamegui/viewport.h>

int scrollbar_width=16;

static gg_colour_t col_red =
    {
        1.0f, 0.0f, 0.0f, 1.0f
    };

static gg_colour_t col_blue =
    {
        0.0f, 0.0f, 1.0f, 1.0f
    };

static gg_colour_t col_green =
    {
        0.0f, 1.0f, 0.0f, 1.0f
    };

gg_class_id gg_scrollbox_get_class_id()
{
    GG_CHILD(gg_bin_get_class_id())
}

/** Implements widget::render for scrollbox widgets. */
void gg_scrollbox_render(gg_widget_t *widget, int x, int y, int focus)
{
    gg_widget_t *child = gg_bin_get_child(GG_BIN(widget));
    gg_scrollbox_t *scrollbox = GG_SCROLLBOX(widget);
    gg_widget_t *view_child = gg_bin_get_child(GG_BIN(child));
    int sliderpos=1;
    int slidersize=16;
    float yscroll=GG_VIEWPORT(child)->yscroll;

    /* Draw scrollbar.. */
    gg_system_draw_gradient_rect(x+widget->width-16.0f, y, 16, 16, 
        &col_red, &col_red, &col_red, &col_red, FALSE );

    gg_system_draw_gradient_rect(x+widget->width-16.0f, y+16, 16, widget->height-(32), 
        &col_blue, &col_blue, &col_blue, &col_blue, FALSE );

    gg_system_draw_gradient_rect(x+widget->width-16.0f, y+widget->height-16, 16, 16, 
        &col_red, &col_red, &col_red, &col_red, FALSE );

    /* Draw the slider! */
    slidersize=((float)child->height/(float)view_child->height)*(widget->height-32);
    sliderpos=((widget->height-(32)+1-slidersize)*yscroll)-1;
    gg_system_draw_gradient_rect(x+widget->width-16.0f, y+16+sliderpos, 16, slidersize, 
        &col_green, &col_green, &col_green, &col_green, FALSE );

    child->render(child, x, y, GG_FOCUS_NONE);
}

/** Implements widget::input for scrollbox widgets. */
int gg_scrollbox_input(gg_widget_t *widget, gg_event_t event)
{
    gg_widget_t *child = gg_bin_get_child(GG_BIN(widget));
    gg_scrollbox_t *scrollbox = GG_SCROLLBOX(widget);
    gg_widget_t *view_child = gg_bin_get_child(GG_BIN(child));
    float yscroll=GG_VIEWPORT(child)->yscroll;
    int sliderpos=1;
    int slidersize=16;
    float newpos=0;

    if ( event.type == GG_EVENT_MOUSE && event.mouse.x>(widget->width-16) && 
        event.mouse.x<(widget->width) )
    {
        if ( event.mouse.y < 16 && yscroll-0.05>=0.0f && event.mouse.type == GG_MOUSE_BUTTON_UP )
            gg_viewport_set_scroll_ypos(GG_VIEWPORT(child), yscroll-=0.05 );

        if ( event.mouse.y > widget->height-16 && yscroll+0.05<=1.0f && event.mouse.type == GG_MOUSE_BUTTON_UP )
            gg_viewport_set_scroll_ypos(GG_VIEWPORT(child), yscroll+=0.05 );

        /* Are we over the grabber? */
        slidersize=((float)child->height/(float)view_child->height)*(widget->height-32);
        sliderpos=((widget->height-(32)+1-slidersize)*(1.0-yscroll))-1;

        if ( event.mouse.y >= widget->height-16-slidersize-sliderpos && event.mouse.y <= 
            widget->height-16-sliderpos && event.mouse.type == GG_MOUSE_BUTTON_DOWN )
        {
            GG_SCROLLBOX(widget)->grabbed=TRUE;          
        } 
      
        if ( event.mouse.type == GG_MOUSE_BUTTON_UP )      
        {
            GG_SCROLLBOX(widget)->grabbed=FALSE;   
        }

        if ( GG_SCROLLBOX(widget)->grabbed )
        {      
            newpos=((float)event.mouse.y-16.0f)/((float)widget->height-32.0f);            

            if ( newpos >= 0.0f && newpos <= 1.0f )
                gg_viewport_set_scroll_ypos(GG_VIEWPORT(child), ((float)event.mouse.y-16.0f)/((float)widget->height-32.0f) );
            /*printf( "Moved %f / %f = %f\n", (float)(event.mouse.y-16.0f), (widget->height-32), GG_VIEWPORT(child)->yscroll );*/
        }

        return 1;
    }
    else
        return child->input(child, event);

    return 0;
}

void gg_scrollbox_init(gg_scrollbox_t *scrollbox, gg_widget_t *widget)
{
    gg_bin_init((gg_bin_t *) scrollbox, widget);

    scrollbox->render = gg_scrollbox_render;
    scrollbox->input = gg_scrollbox_input;
    scrollbox->id = gg_scrollbox_get_class_id();
    scrollbox->enabled = 1;
    scrollbox->grabbed=FALSE;
    scrollbox->width = widget->width+scrollbar_width; /* FIXME */
    scrollbox->height = widget->height; /* FIXME */
}

gg_widget_t *gg_scrollbox_create(gg_widget_t *widget)
{
    gg_scrollbox_t *scrollbox = malloc(sizeof(gg_scrollbox_t));

    gg_scrollbox_init(scrollbox, widget);

    return GG_WIDGET(scrollbox);
}
