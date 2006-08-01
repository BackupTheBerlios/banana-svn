
#include "main.h"

static void tile_selected(gg_widget_t *widget, void *data)
{
    int tilenr,slot;
    tilenr = ((int)GG_IMAGE(gg_bin_get_child(GG_BIN(widget)))->image-(int)get_tilebay_addr())/sizeof(texture_t);
    slot=(int)data;

    set_tile_slot(slot,tilenr);
    gg_dialog_close();
}

int slot_to_set=1;
static gg_dialog_t *create_tile_select_dialog(int slot)
{
    gg_widget_t *dialog;
    gg_widget_t *vbox;
    gg_widget_t *vbox2;
    gg_widget_t *hbox;
    gg_widget_t *widget;
    gg_widget_t *widget2;
    gg_widget_t *view;
    int i,j,k;
    int tiles_wide=6;
    int tiles_high=15;
    slot_to_set=slot;

    k=0;
    vbox2=gg_vbox_create(0);
    for ( i=0; i<tiles_high; i++ )
    {
        hbox = gg_hbox_create(0);
        for ( j=0; j<tiles_wide; j++ )
        {
            if ( k < get_tile_count() )
            {
                widget = gg_image_create(get_tile_tex(k));
                widget2 = gg_action_create(widget);
                gg_action_set_callback(GG_ACTION(widget2), tile_selected, (int*)slot_to_set);
                gg_container_append(GG_CONTAINER(hbox), widget2);
            }
            k++;
        }
        gg_container_append(GG_CONTAINER(vbox2), hbox);
    }

    view=gg_viewport_create(vbox2,tiles_wide*32,96);
    vbox = gg_scrollbox_create(view);

    dialog = gg_dialog_create(vbox, "Tiles", TILE_SELECT_DIALOG);
    gg_dialog_set_modal(GG_DIALOG(dialog), TRUE );
    gg_dialog_set_position(GG_DIALOG(dialog), 320, 200, 0.5f, 0.0f);
    gg_dialog_set_style(GG_DIALOG(dialog), get_menu_style() );

    return GG_DIALOG(dialog);
}

void show_tile_select_dialog( int slot )
{   
    if ( !gg_dialog_is_open_by_id( TILE_SELECT_DIALOG ) )
    gg_dialog_open(create_tile_select_dialog(slot));
}
