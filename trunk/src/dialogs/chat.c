
#include "main.h"

static void send_chat(gg_widget_t *widget, void *data)
{
    gg_entry_t *entry=GG_ENTRY(widget);
    char temp[512];

    if ( strlen(entry->text) > 0 )
    {
        sprintf(temp,"%s: %s", get_player(get_local_player_index())->name, entry->text );
    
        add_chat_buffer_line( temp );
        send_chat_message( temp );
    }

    /* Close window. */
    gg_dialog_close();
}

static gg_dialog_t *create_chat_dialog( int modal )
{
    gg_widget_t *dialog;
    gg_widget_t *vbox;
    gg_widget_t *widget;

    vbox = gg_vbox_create(0);

    widget = gg_entry_create();
    gg_entry_set_enter_callback(GG_ENTRY(widget), send_chat, NULL); 
    gg_set_requested_size(widget,500,0);
    gg_container_append(GG_CONTAINER(vbox), widget);
    
    dialog = gg_dialog_create(vbox, NULL, CHAT_DIALOG);
    gg_dialog_set_position(GG_DIALOG(dialog), 320, 375, 0.5f, 0.0f);
    gg_dialog_set_style(GG_DIALOG(dialog), get_menu_style() );

    return GG_DIALOG(dialog);
}

void show_chat_dialog( int modal )
{   
   /* if (!gg_dialog_current()) */
        gg_dialog_open(create_chat_dialog(modal));
}
