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
#include <stdio.h>

#include <gamegui/dialog.h>

static gg_colour_t col_white =
    {
        1.0f, 1.0f, 1.0f, 1.0f
    };

static gg_colour_t col_black =
    {
        0.0f, 0.0f, 0.0f, 1.0f
    };

static gg_colour_t col_blue =
    {
        0.0f, 0.5f, 1.0f, 1.0f
    };

static gg_colour_t col_blue2 =
    {
        0.0f, 0.0f, 0.5f, 1.0f
    };

static gg_colour_t col_grey =
    {
        0.5f, 0.5f, 0.5f, 1.0f
    };

static gg_colour_t col_grey2 =
    {
        0.25f, 0.25f, 0.25f, 1.0f
    };

gg_class_id gg_dialog_get_class_id()
{
    GG_CHILD(gg_bin_get_class_id())
}

/** The maximum amount of open dialogs that the dialog system can handle. */
#define DIALOG_MAX 10

/** The dialog stack. */
gg_dialog_t *dialog_stack[DIALOG_MAX];

/** The amount of dialogs that are currently open. */
int dialog_nr = 0;

/** To-be-destroyed dialogs. */
gg_dialog_t *dialog_closed[DIALOG_MAX];

/** The amount of dialogs that need to be destroyed. */
int dialog_closed_nr = 0;

float global_dialog_trans=1.0f;

int gg_dialog_is_open_by_id( int id )
{
    int i=0;

    /* Check for an instance of a particular dialog ID. */
    for (i=0; i<dialog_nr-1; i++)
    {
        if ( dialog_stack[i]->dialog_id == id )
            return TRUE;
    }

    return FALSE;
}

void gg_dialog_set_window_id( gg_dialog_t *dialog, int id )
{
    dialog->dialog_id=id;
}

void gg_dialog_cleanup()
{
    int i;

    for (i = 0; i < dialog_closed_nr; i++)
    {
        gg_dialog_t *dialog = dialog_closed[i];
        dialog->destroy(GG_WIDGET(dialog));
    }

    dialog_closed_nr = 0;
}

void gg_dialog_move_to_front( int index )
{
    gg_dialog_t *d=dialog_stack[index]; 
    int i=0;

    for (i=index; i<dialog_nr-1; i++ )
    {
        dialog_stack[i]=dialog_stack[i+1];
    }
    dialog_stack[dialog_nr-1]=d;
}

/** @brief Adds a dialog to the top of the dialog stack.
 *  @param menu The dialog to add.
 */
void gg_dialog_open(gg_dialog_t *menu)
{
    if (dialog_nr == DIALOG_MAX)
    {
        printf("Too many open dialogs.\n");
        return;
    }

    if ( gg_dialog_current() && gg_dialog_current()->modal )
    {
        dialog_stack[dialog_nr++] = menu;
        gg_dialog_move_to_front(dialog_nr-2);        
    }
    else
    {
        dialog_stack[dialog_nr++] = menu;
        gg_dialog_move_to_front(dialog_nr-1);
    }
}

/** @brief Closes the dialog that's on top of the dialog stack. */
void gg_dialog_close()
{
    gg_dialog_t *menu;

    if (dialog_nr == 0)
    {
        printf("No open dialogs.\n");
        return;
    }

    menu = dialog_stack[dialog_nr-- - 1];

    if (dialog_closed_nr == DIALOG_MAX)
    {
        printf("Too many to-be-destroyed dialogs.\n");
        return;
    }

    dialog_closed[dialog_closed_nr++] = menu;
}

/** @brief Returns the dialog that's on top of the stack.
 *
 *  @return The dialog that's on top of the stack, or NULL if the stack is
 *          empty.
 */
gg_dialog_t *gg_dialog_current()
{
    if (dialog_nr == 0)
        return NULL;

    return dialog_stack[dialog_nr - 1];
}

int gg_dialog_count()
{
    return dialog_nr;
}

gg_dialog_t *gg_get_dialog( int index )
{
    if (index >= dialog_nr)
        return NULL;

    return dialog_stack[index];
}

void gg_dialog_set_trans( float trans )
{
    global_dialog_trans=trans;
}

float gg_dialog_trans()
{
    return global_dialog_trans;
}

void gg_dialog_get_screen_pos(gg_dialog_t *dialog, int *x, int *y)
{
    *x = dialog->pos.x - dialog->width * dialog->pos.x_align;
    *y = dialog->pos.y - dialog->height * dialog->pos.y_align;
}

void draw_border( void *image[9], gg_rect_t area, int size, char *titlebar, int active)
{
    gg_rect_t source, dest;
    gg_colour_t fade_col={1.0f,1.0f,1.0f,0.8f};
    int titlebar_height=0;

    source.x=source.y=0;
    source.width=source.height=size;

    if (titlebar) /* Draw a titlebar */
    {    
        gui_get_string_size(titlebar, NULL, &titlebar_height);
        titlebar_height+=(titlebar_height/4)+1;

        dest.x = area.x; dest.y = area.y+area.height;
        dest.width = area.width; dest.height = titlebar_height;

        gui_draw_rect_fill(dest.x, dest.y, dest.width, dest.height, 
            &col_black, &col_black, &col_black, &col_black, FALSE );

        if ( active )
            gg_system_draw_gradient_rect( dest.x, dest.y+1, dest.width, dest.height, 
                &col_blue2, &col_blue, &col_blue2, &col_blue, FALSE );
        else
            gg_system_draw_gradient_rect( dest.x, dest.y+1, dest.width, dest.height, 
                &col_grey2, &col_grey, &col_grey2, &col_grey, FALSE );

        gg_system_draw_string(titlebar, dest.x+10, dest.y+(titlebar_height/8), &col_white, 0, 1, 0);
    }

    /* Draw Top Left Corner */
    dest.x = area.x-size; dest.y = area.y+area.height+titlebar_height;
    dest.width = size; dest.height = size;
    gg_system_draw_image(image[0], source, dest, GG_MODE_TILE, GG_MODE_SCALE, &col_white, FALSE);

    /* Draw Top Middle */
    dest.x = area.x; dest.y = area.y+area.height+titlebar_height;
    dest.width = area.width; dest.height = size;
    gg_system_draw_image(image[1], source, dest, GG_MODE_TILE, GG_MODE_SCALE, &col_white, FALSE);

    /* Draw Top Right Corner */
    dest.x = area.x+area.width; dest.y = area.y+area.height+titlebar_height;
    dest.width = size; dest.height = size;
    gg_system_draw_image(image[2], source, dest, GG_MODE_TILE, GG_MODE_SCALE, &col_white, FALSE);
    
    /* Draw Left Side */
    dest.x = area.x-size; dest.y = area.y;
    dest.width = size; dest.height = area.height+titlebar_height;
    gg_system_draw_image(image[3], source, dest, GG_MODE_TILE, GG_MODE_SCALE, &col_white, FALSE);

    /* Draw Right Side */
    dest.x = area.x+area.width; dest.y = area.y;
    dest.width = size; dest.height = area.height+titlebar_height;
    gg_system_draw_image(image[5], source, dest, GG_MODE_TILE, GG_MODE_SCALE, &col_white, FALSE);

    /* Draw Bottom Left Corner */
    dest.x = area.x-size; dest.y = area.y-size;
    dest.width = size; dest.height = size;
    gg_system_draw_image(image[6], source, dest, GG_MODE_TILE, GG_MODE_SCALE, &col_white, FALSE);

    /* Draw Bottom Middle */
    dest.x = area.x; dest.y = area.y-size;
    dest.width = area.width; dest.height = size;
    gg_system_draw_image(image[7], source, dest, GG_MODE_TILE, GG_MODE_SCALE, &col_white, FALSE);

    /* Draw Bottom Right Corner */
    dest.x = area.x+area.width; dest.y = area.y-size;
    dest.width = size; dest.height = size;
    gg_system_draw_image(image[8], source, dest, GG_MODE_TILE, GG_MODE_SCALE, &col_white, FALSE);

    /* Draw middle */
    dest.x = area.x; dest.y = area.y;
    dest.width = area.width; dest.height = area.height;
    gg_system_draw_image(image[4], source, dest, GG_MODE_TILE, GG_MODE_TILE, &fade_col, FALSE);
}

/** @brief Renders a dialog.
 *
 *  Renders a dialog in a specific style and at a specific position.
 *
 *  @param menu The dialog to render.
 *  @param style The style to render in.
 *  @param pos The position to render at.
 */
void gg_dialog_render(gg_dialog_t *dialog)
{
    gg_dialog_style_t *style = &dialog->style;
    gg_widget_t *child = gg_bin_get_child(GG_BIN(dialog));

    int xmin, xmax, ymin, ymax;

    gg_dialog_get_screen_pos(dialog, &xmin, &ymin);

    xmax = xmin + dialog->width;
    ymax = ymin + dialog->height;

    /* Draw the 'fade' */
    //gg_system_draw_filled_rect(0, 0, 640, 480, &style->fade_col);

    if (style->textured)
    {
        int size;
        gg_rect_t area;
        int active;

        gg_system_get_image_size(style->border.textured.image[0], &size, NULL);

        xmin += size;
        xmax -= size;
        ymin += size;
        ymax -= size;

        area.x = xmin;
        area.y = ymin;
        area.width = xmax - xmin;
        area.height = ymax - ymin;

        if ( dialog == gg_dialog_current() )
            active=TRUE;
        else
            active=FALSE;        

        draw_border(style->border.textured.image, area, size, dialog->titlebar, active);
    }
    else
    {
        /* Draw the border. */
        gg_system_draw_filled_rect(xmin, ymin, xmax - xmin, ymax - ymin, &style->border.plain.border_col);

        xmin += style->border.plain.border;
        xmax -= style->border.plain.border;
        ymin += style->border.plain.border;
        ymax -= style->border.plain.border;

        /* Draw the backdrop. */
        gg_system_draw_filled_rect(xmin, ymin, xmax - xmin, ymax - ymin, &style->border.plain.bg_col);
    }

    xmin += style->hor_pad;
    xmax -= style->hor_pad;
    ymin += style->vert_pad;
    ymax -= style->vert_pad;

    child->render(child, xmin, ymin, 1);
}

void gg_dialog_mouse_movement(gg_dialog_t *dialog, int x, int y)
{
    gg_widget_t *child = gg_bin_get_child(GG_BIN(dialog));
    int xmin, xmax, ymin, ymax;
    int xoffadj, yoffadj;

    gg_dialog_get_screen_pos(dialog, &xmin, &ymin);

    if (dialog->titlebar && dialog->moving )
    {
        xoffadj=dialog->width*dialog->pos.x_align;
        yoffadj=dialog->height*dialog->pos.y_align;

        gg_dialog_set_position(dialog, x-dialog->move_xoffset+xoffadj, y-dialog->move_yoffset+yoffadj, 
            dialog->pos.x_align, dialog->pos.y_align);
    }

    xmax = xmin + dialog->width;
    ymax = ymin + dialog->height;

    if ((x < xmin) || (x >= xmax) || (y < ymin) || (y >= ymax))
        return;

    x -= xmin + dialog->style.hor_pad;
    y -= ymin + dialog->style.vert_pad;

    if (dialog->style.textured)
    {
        int size;

        gg_system_get_image_size(dialog->style.border.textured.image[0], &size, NULL);
        x -= size;
        y -= size;
    }

    if (x >= 0 && x < child->width_a && y >= 0 && y < child->height_a)
        dialog->set_focus_pos(GG_WIDGET(dialog), x, y);
}

int gg_dialog_input(gg_widget_t *widget, gg_event_t event)
{
    gg_dialog_t *dialog = GG_DIALOG(widget);
    gg_widget_t *child = gg_bin_get_child(GG_BIN(widget));
    int x, y, size, titlebar_height;

    /*printf( "Mouseb: %i\n", event.mouse.button );*/
   
    if (!dialog->modal && event.type == GG_EVENT_KEY && event.key == GG_KEY_ESCAPE )
        gg_dialog_close();

    gg_dialog_get_screen_pos(dialog, &x, &y);

    if (event.type == GG_EVENT_MOUSE)
    {
        if (dialog->titlebar)
        {
            if ( event.mouse.type == GG_MOUSE_BUTTON_UP )
                dialog->moving=FALSE;

            gg_system_get_image_size(dialog->style.border.textured.image[0], NULL, &size);
            gui_get_string_size(dialog->titlebar, NULL, &titlebar_height);
            titlebar_height+=(titlebar_height/4)+1;

            if ( event.mouse.x>=x+size && event.mouse.x<=x+dialog->width-size )
            if ( event.mouse.y>=y+dialog->height-size && event.mouse.y<=y+dialog->height-size+titlebar_height)
            {
                if ( event.mouse.type == GG_MOUSE_BUTTON_DOWN )
                {
                    dialog->move_xoffset=event.mouse.x-x;
                    dialog->move_yoffset=event.mouse.y-y;

                    dialog->moving=TRUE;
                }
            }
        }
    
        event.mouse.x -= x + dialog->style.hor_pad;
        event.mouse.y -= y + dialog->style.vert_pad;

        if (dialog->style.textured)
        {
            int size;

            gg_system_get_image_size(dialog->style.border.textured.image[0], &size, NULL);
            event.mouse.x -= size;
            event.mouse.y -= size;
        }

        if (event.mouse.x < 0 || event.mouse.x >= child->width_a ||
            event.mouse.y < 0 || event.mouse.y >= child->height_a)
            return 0;
    }

    return child->input(child, event);
}

int gg_dialog_mouse_over( gg_dialog_t * dialog, int xpos, int ypos )
{
    int x,y, titlebar_height;;
    gg_dialog_get_screen_pos(dialog, &x, &y);
    gui_get_string_size(dialog->titlebar, NULL, &titlebar_height);
    titlebar_height+=(titlebar_height/4)+1;

    if ( dialog->titlebar && xpos>x && xpos<(x+dialog->width) && ypos>y && ypos<(y+dialog->height+titlebar_height) )
        return TRUE;
    else if ( xpos>x && xpos<(x+dialog->width) && ypos>y && ypos<(y+dialog->height) )
        return TRUE;
    else
        return FALSE;
}

int gg_dialog_mouse_over_any( int x, int y )
{
    int i=0, retval=FALSE;

    for (i=dialog_nr-1; i>=0; i-- )    
    {
        if ( gg_dialog_mouse_over(dialog_stack[i], x, y ) || dialog_stack[i]->moving )
        {
            retval=TRUE;
        }
    }    
    
    return retval;
}

void gg_dialog_input_current(gg_event_t event)
{
    gg_dialog_t *dialog;
    int i=0;

    /* Which dialog is actually current? hmm */
    if ( !gg_dialog_current()->modal )
    for (i=dialog_nr-1; i>=0; i-- )    
    {
        /*printf( "We have dialog[%i] at %i,%i\n", i, dialog_stack[i]->pos.x, dialog_stack[i]->pos.x );*/
        if ( gg_dialog_mouse_over(dialog_stack[i], event.mouse.x, event.mouse.y ) )
        {
            /*printf( "Dialog[%i] is now active - %i\n", i, SDL_GetTicks() );*/
            gg_dialog_move_to_front( i );
            break;
        }
            /*dialog=dialog_stack[i];*/
    }

    dialog = gg_dialog_current();

    if (!dialog)
        return;

    gg_dialog_input(GG_WIDGET(dialog), event);
}

void gg_dialog_set_modal(gg_dialog_t *dialog, int modal)
{
    dialog->modal = modal;
}

void gg_dialog_set_position(gg_dialog_t *dialog, int x, int y, float x_align, float y_align)
{
    dialog->pos.x = x;
    dialog->pos.y = y;
    dialog->pos.x_align = x_align;
    dialog->pos.y_align = y_align;
}

void gg_dialog_init(gg_dialog_t *dialog, gg_widget_t *child, char *titlebar, int id)
{
    gg_dialog_style_t style;
    gg_colour_t border_col = {0.0f, 0.0f, 0.0f, 1.0f};
    gg_colour_t bg_col = {0.8f, 0.8f, 0.8f, 1.0f};
    gg_colour_t fade_col = {0.0f, 0.0f, 0.0f, 0.5f};

    style.textured = 0;
    style.border.plain.border = 5;
    style.border.plain.border_col = border_col;
    style.border.plain.bg_col = bg_col;
    style.fade_col = fade_col;
    style.hor_pad = 20;
    style.vert_pad = 10;

    gg_bin_init((gg_bin_t *) dialog, child);

    dialog->input = gg_dialog_input;
    dialog->id = gg_dialog_get_class_id();
    dialog->modal = 0;
    if (titlebar)
        dialog->titlebar = strdup(titlebar);
    else
        dialog->titlebar=NULL;
    dialog->moving=FALSE;
    dialog->move_xoffset=0;
    dialog->move_yoffset=0;
    dialog->dialog_id=id;

    child->get_requested_size(child, &dialog->width, &dialog->height);
    child->set_size(child, dialog->width, dialog->height);
    gg_dialog_set_style(dialog, &style);

    gg_dialog_set_position(dialog, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.5f, 0.5f);
}

/** @brief Creates a dialog.
 *
 *  @param widget The widget the dialog contains.
 *  @return The created dialog.
 */
gg_widget_t *gg_dialog_create(gg_widget_t *child, char *titlebar, int id)
{
    gg_dialog_t *dialog = malloc(sizeof(gg_dialog_t));

    gg_dialog_init(dialog, child, titlebar, id);

    return GG_WIDGET(dialog);
}

void gg_dialog_set_style(gg_dialog_t *dialog, gg_dialog_style_t *style)
{
    gg_widget_t *child = gg_bin_get_child(GG_BIN(dialog));

    dialog->style = *style;
    dialog->width = child->width_a + 2 * dialog->style.hor_pad;
    dialog->height = child->height_a + 2 * dialog->style.vert_pad;

    if (style->textured)
    {
        int size;

        gg_system_get_image_size(style->border.textured.image[0], &size, NULL);
        dialog->width += 2 * size;
        dialog->height += 2 * size;
    }
    else
    {
        dialog->width += 2 * dialog->style.border.plain.border;
        dialog->height += 2 * dialog->style.border.plain.border;
    }
}
