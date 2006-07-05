
#include "main.h"

static gg_colour_t col_trans =
{
    1.0f, 1.0f, 1.0f, 0.0f
};

static gg_colour_t col_white =
{
    1.0f, 1.0f, 1.0f, 1.0f
};

static char client_ip[80];
static int client_port;
static char client_nickname[80];

static void close(gg_widget_t *widget, void *data)
{
    gg_dialog_close();
}

static void ip_changed(gg_widget_t *widget, void *data)
{
    gg_entry_t *entry=GG_ENTRY(widget);

    sprintf(client_ip,"%s", entry->text );
}

static void port_changed(gg_widget_t *widget, void *data)
{
    gg_entry_t *entry=GG_ENTRY(widget);

    client_port=atoi(entry->text);
}

static void nick_changed(gg_widget_t *widget, void *data)
{
    gg_entry_t *entry=GG_ENTRY(widget);

    strcpy(client_nickname,entry->text);
}

static void start(gg_widget_t *widget, void *data)
{
    connect_to_server(client_ip,client_port,client_nickname);
    gg_dialog_close();
}

static gg_dialog_t *create_client_dialog( int modal )
{
    gg_widget_t *dialog;
    gg_widget_t *vbox;
    gg_widget_t *hbox;
    gg_widget_t *widget;

    vbox = gg_vbox_create(0);

    hbox = gg_hbox_create(0);  
    widget = gg_button_create("Cancel");
    gg_action_set_callback(GG_BUTTON(widget), close, NULL);        
    gg_container_append(GG_CONTAINER(hbox), widget);
    widget = gg_button_create("Start");
    gg_action_set_callback(GG_BUTTON(widget), start, NULL);        
    gg_container_append(GG_CONTAINER(hbox), widget);
    gg_container_append(GG_CONTAINER(vbox), hbox);

    hbox=gg_hbox_create(0);
    widget = gg_label_create("Port: ");
    gg_label_set_colour(GG_LABEL(widget), &col_white, &col_trans );
    gg_container_append(GG_CONTAINER(hbox), widget);
    widget = gg_entry_create();
    gg_entry_set_change_callback(GG_ENTRY(widget), port_changed, NULL); 
    gg_container_append(GG_CONTAINER(hbox), widget);
    gg_container_append(GG_CONTAINER(vbox), hbox);
    
    hbox=gg_hbox_create(0);
    widget = gg_label_create("IP: ");
    gg_label_set_colour(GG_LABEL(widget), &col_white, &col_trans );
    gg_container_append(GG_CONTAINER(hbox), widget);
    widget = gg_entry_create();
    gg_entry_set_change_callback(GG_ENTRY(widget), ip_changed, NULL); 
    gg_container_append(GG_CONTAINER(hbox), widget);
    gg_container_append(GG_CONTAINER(vbox), hbox);

    hbox=gg_hbox_create(0);
    widget = gg_label_create("Nickname: ");
    gg_label_set_colour(GG_LABEL(widget), &col_white, &col_trans );
    gg_container_append(GG_CONTAINER(hbox), widget);
    widget = gg_entry_create();
    gg_entry_set_change_callback(GG_ENTRY(widget), nick_changed, NULL); 
    gg_container_append(GG_CONTAINER(hbox), widget);
    gg_container_append(GG_CONTAINER(vbox), hbox);

    widget = gg_label_create("Connect to server.");
    gg_label_set_colour(GG_LABEL(widget), &col_white, &col_trans );
    gg_container_append(GG_CONTAINER(vbox), widget);

    dialog = gg_dialog_create(vbox);
    if (modal)
        gg_dialog_set_modal(GG_DIALOG(dialog), 1);
    gg_dialog_set_position(GG_DIALOG(dialog), 320, 300, 0.5f, 0.0f);
    gg_dialog_set_style(GG_DIALOG(dialog), get_menu_style() );

    return GG_DIALOG(dialog);
}

void show_client_dialog( int modal )
{
    //if (modal)
        //grab_framebuffer();    
 
    if (!gg_dialog_current())
        gg_dialog_open(create_client_dialog(modal));
}
