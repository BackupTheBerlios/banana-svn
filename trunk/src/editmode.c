
#include "main.h"

static gg_colour_t col_white =
{
    1.0f, 1.0f, 1.0f, 1.0f
};

static int editmode=EDITING_PLAYER_LAYER;
static int editing=FALSE;

int get_editmode()
{
    return editmode;
}

int get_editing()
{
    return editing;
}

void set_editing( int edit )
{
    editing=edit;
}

void draw_edit_widgets()
{
    char temp_str[80];
    gg_rect_t area;
    int size;

    gg_system_get_image_size(get_menu_style()->border.textured.image[0], &size, NULL);
    area.x = 0; area.y = 0;

    /* Draw the layer widget */
    glPushMatrix();
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 

    sprintf( temp_str, "Editing: Obstruction Layer" );
    gui_get_string_size(temp_str, &area.width, &area.height);
    area.width+=size;

    glTranslatef( 8.0f, 7.0f, 0.0f );
    draw_border(get_menu_style()->border.textured.image, area, size, NULL); 
    gg_system_draw_string(temp_str, 5, 1, &col_white, 0, 0, 0);

    glDisable(GL_BLEND);
    glPopMatrix();

}
