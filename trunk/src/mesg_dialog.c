
#include "main.h"

static gg_colour_t col_white =
{
    1.0f, 1.0f, 1.0f, 1.0f
};

static gg_colour_t col_trans =
{
    1.0f, 1.0f, 1.0f, 0.0f
};

static void close(gg_widget_t *widget, void *data)
{
    gg_dialog_close();
}

static gg_dialog_t *create_mesg_dialog( char *message )
{
    gg_widget_t *dialog;
    gg_widget_t *vbox;
    gg_widget_t *widget;

    vbox = gg_vbox_create(0);

    widget = gg_button_create("Ok");
    gg_button_set_callback(GG_BUTTON(widget), close, NULL);        
    gg_container_append(GG_CONTAINER(vbox), widget);
    widget = gg_label_create(message);
    /*gg_set_requested_size(widget,500,0);*/
    gg_label_set_colour(GG_LABEL(widget), &col_white, &col_trans );
    gg_container_append(GG_CONTAINER(vbox), widget);
    
    dialog = gg_dialog_create(vbox, "Error");
    gg_dialog_set_position(GG_DIALOG(dialog), 320, 375, 0.5f, 0.0f);
    gg_dialog_set_style(GG_DIALOG(dialog), get_menu_style() );

    return GG_DIALOG(dialog);
}

void show_mesg_dialog( char *message )
{   
    gg_dialog_open(create_mesg_dialog(message));
}
