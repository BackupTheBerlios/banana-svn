
#include "main.h"

/*static gg_colour_t col_white =
{
    1.0f, 1.0f, 1.0f, 1.0f
};

static gg_colour_t col_trans =
{
    1.0f, 1.0f, 1.0f, 0.0f
};*/

gg_widget_t *slots[3];

void change_current_tiles_dialog_slot( int slot, int index )
{
    if (slots[slot])
        GG_IMAGE(slots[slot])->image=get_tile_tex(index);
}

static void open_selector(gg_widget_t *widget, void *data)
{
    show_tile_select_dialog((int)data);
}

static void edit_layer_changed(gg_widget_t *widget, void *data)
{
    printf( "Now selected be %i\n", GG_OPTION(widget)->sel );
    
}

static gg_dialog_t *create_current_tiles_dialog()
{
    gg_widget_t *dialog;
    gg_widget_t *hbox;
    gg_widget_t *widget;
    gg_widget_t *vbox;

    hbox = gg_hbox_create(0);

    slots[0] = gg_image_create(get_tile_tex(get_tile_slot(0)));
    widget = gg_action_create(slots[0]);
    gg_action_set_callback(GG_ACTION(widget), open_selector, (int*)0);
    gg_container_append(GG_CONTAINER(hbox), widget);

    slots[1] = gg_image_create(get_tile_tex(get_tile_slot(1)));
    widget = gg_action_create(slots[1]);
    gg_action_set_callback(GG_ACTION(widget), open_selector, (int*)1);
    gg_container_append(GG_CONTAINER(hbox), widget);

    slots[2] = gg_image_create(get_tile_tex(get_tile_slot(2)));
    widget = gg_action_create(slots[2]);
    gg_action_set_callback(GG_ACTION(widget), open_selector, (int*)2);
    gg_container_append(GG_CONTAINER(hbox), widget);
 
    vbox = gg_vbox_create(0);
    gg_container_append(GG_CONTAINER(vbox), hbox);

    widget = gg_option_create();
    gg_option_append_label(GG_OPTION(widget), "PL", 0.5f, 0.0f);
    gg_option_append_label(GG_OPTION(widget), "FG", 0.5f, 0.0f);
    gg_option_append_label(GG_OPTION(widget), "Obs", 0.5f, 0.0f);

    gg_option_set_callback(GG_OPTION(widget), edit_layer_changed, NULL);
    gg_container_append(GG_CONTAINER(vbox), widget);

    dialog = gg_dialog_create(vbox, "Tiles", CURRENT_TILES_DIALOG);
    gg_dialog_set_position(GG_DIALOG(dialog), 510, 375, 0.0f, 0.0f);
    gg_dialog_set_style(GG_DIALOG(dialog), get_menu_style() );

    return GG_DIALOG(dialog);
}

void show_current_tiles_dialog()
{   
    gg_dialog_open(create_current_tiles_dialog());
}
