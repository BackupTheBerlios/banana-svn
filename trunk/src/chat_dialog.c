
#include "main.h"

static gg_colour_t col_trans =
    {
        0.0f, 0.0f, 0.0f, 0.0f
    };

static gg_colour_t col_white =
    {
        1.0f, 1.0f, 1.0f, 1.0f
    };

static void close(gg_widget_t *widget, void *data)
{
    gg_dialog_close();
}

static void send_chat(gg_widget_t *widget, void *data)
{
    gg_entry_t *entry=GG_ENTRY(widget);

    if ( strlen(entry->text) > 0 )
    {
        add_chat_buffer_line( entry->text );
        send_chat_message( entry->text );

        entry->text[0] = '\0';
        entry->cursor_pos=0;
    }
}

static gg_dialog_t *create_chat_dialog( int modal )
{
    gg_widget_t *dialog;
    gg_widget_t *vbox;
    gg_widget_t *hbox;
    gg_widget_t *widget;

    vbox = gg_vbox_create(0);

    widget = gg_button_create("Close");
    gg_action_set_callback(GG_BUTTON(widget), close, NULL);        
    gg_container_append(GG_CONTAINER(vbox), widget);

    hbox=gg_hbox_create(0);
    widget = gg_entry_create();
    gg_entry_set_enter_callback(GG_ENTRY(widget), send_chat, NULL); 
    gg_set_requested_size(widget,500,0);
    gg_container_append(GG_CONTAINER(vbox), widget);
    
    dialog = gg_dialog_create(vbox);
    if (modal)
        gg_dialog_set_modal(GG_DIALOG(dialog), 1);
    gg_dialog_set_position(GG_DIALOG(dialog), 320, 300, 0.5f, 0.0f);
    gg_dialog_set_style(GG_DIALOG(dialog), get_menu_style() );

    return GG_DIALOG(dialog);
}

void show_chat_dialog( int modal )
{
    if (modal)
        grab_framebuffer();     
    gg_dialog_open(create_chat_dialog(modal));
}
