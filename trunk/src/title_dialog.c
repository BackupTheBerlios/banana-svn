
#include "main.h"

static gg_colour_t col_white =
{
    1.0f, 1.0f, 1.0f, 1.0f
};

static gg_colour_t col_trans =
{
    1.0f, 1.0f, 1.0f, 0.0f
};

static void kaboomy(gg_widget_t *widget, void *data)
{
    printf( "Kaboomy!" );
    exit(0);
}

static gg_dialog_t *create_title_dialog( int modal )
{
    gg_widget_t *dialog;
    gg_widget_t *vbox;
    gg_widget_t *widget;

    vbox = gg_vbox_create(0);
    if ( !modal )
    {
        widget = gg_button_create("Quit");
        gg_button_set_callback(GG_BUTTON(widget), kaboomy, NULL);        
        gg_container_append(GG_CONTAINER(vbox), widget);
        widget = gg_entry_create();
        gg_container_append(GG_CONTAINER(vbox), widget);
    }
    widget = gg_label_create("Brain twitch.. whoa.. I think i'm slipping..");
    gg_label_set_colour(GG_LABEL(widget), &col_white, &col_trans );
    gg_container_append(GG_CONTAINER(vbox), widget);
    widget = gg_label_create("Endlessly looping ... constantly flipping..");
    gg_label_set_colour(GG_LABEL(widget), &col_white, &col_trans );
    gg_container_append(GG_CONTAINER(vbox), widget);  
    widget = gg_label_create("Round and round we go... forever spinning..");
    gg_label_set_colour(GG_LABEL(widget), &col_white, &col_trans );
    gg_container_append(GG_CONTAINER(vbox), widget);

    dialog = gg_dialog_create(vbox);
    if (modal)
        gg_dialog_set_modal(GG_DIALOG(dialog), 1);
    gg_dialog_set_position(GG_DIALOG(dialog), 320, 300, 0.5f, 0.0f);
    gg_dialog_set_style(GG_DIALOG(dialog), get_menu_style() );

    return GG_DIALOG(dialog);
}

void show_title_dialog( int modal )
{
    if (modal)
        grab_framebuffer();     
    gg_dialog_open(create_title_dialog(modal));
}
