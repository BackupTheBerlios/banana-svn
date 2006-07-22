
#include "main.h"

char chat_buffer[CHAT_LINES][80];
int chat_buffer_lines=0;
int pps_in=0;
int pps_out=0;

int *get_pps_in()
{
    return &pps_in;
}

int *get_pps_out()
{
    return &pps_out;
}

void send_playermoved_message( int index, int xpos, int ypos, int facing )
{
    char buffer[512];
    unsigned int cxpos=xpos;
    unsigned int cypos=ypos;

    buffer[0]=MESG_PLAYERMOVED;
    buffer[1]=index;
    buffer[2]=facing;
    memcpy(buffer+3,&cxpos,sizeof(int));
    memcpy(buffer+7,&cypos,sizeof(int));

    send_message_omit( buffer[1], buffer, 11 );
}

int get_chat_buffer_lines()
{
    return chat_buffer_lines;
}

char *get_chat_buffer_line( int line )
{
    return chat_buffer[line];
}

void add_chat_buffer_line( char *text )
{
    int i=0;

    if ( chat_buffer_lines > CHAT_LINES-1 )
    {
        for (i=1;i<CHAT_LINES;i++)
        {
            strcpy(chat_buffer[i-1],chat_buffer[i]);
        }
        strcpy(chat_buffer[chat_buffer_lines-1], text);
    }
    else
    {
        strcpy(chat_buffer[chat_buffer_lines], text);

        chat_buffer_lines++;
    } 
}

void send_chat_message( char *text )
{
    char buffer[512];

    buffer[0]=MESG_CHAT;
    strcpy(buffer+1,text);

    send_message( buffer, strlen(text)+1 );
}

void send_message_omit( int omited_index, char *buffer, int len )
{
    int i=0;

    if (get_server_active())
    {
        /* Server running. Send message to all connected clients, except the 'omit' index */
        for (i=1;i<MAX_PLAYERS;i++)
        {
            if (get_player_active(i) && i!=omited_index)
            {
                if (SDLNet_TCP_Send(get_client_socket(i), (void *)buffer, 512) < len)
          	    {
          		    printf( "SDLNet_TCP_Send: %s\n", SDLNet_GetError());
           		    exit(EXIT_FAILURE);
           	    }
                pps_out++;
            }
        }

    }
    else if (get_client_active())
    {
        /* Client active. Send message to server */
        if (SDLNet_TCP_Send(get_server_socket(), (void *)buffer, 512) < len)
  	    {
  		    printf( "SDLNet_TCP_Send: %s\n", SDLNet_GetError());
   		    exit(EXIT_FAILURE);
   	    }
        pps_out++;
    }
}

void send_message( char *buffer, int len )
{
    int i=0;

    if (get_server_active())
    {
        /* Server running. Send message to all connected clients */
        for (i=1;i<MAX_PLAYERS;i++)
        {
            if (get_player_active(i))
            {
                if (SDLNet_TCP_Send(get_client_socket(i), (void *)buffer, 512) < len)
          	    {
          		    printf( "SDLNet_TCP_Send: %s\n", SDLNet_GetError());
           		    exit(EXIT_FAILURE);
           	    }
                pps_out++;
            }
        }

    }
    else if (get_client_active())
    {
        /* Client active. Send message to server */
        if (SDLNet_TCP_Send(get_server_socket(), (void *)buffer, 512) < len)
  	    {
  		    printf( "SDLNet_TCP_Send: %s\n", SDLNet_GetError());
   		    exit(EXIT_FAILURE);
   	    }
        pps_out++;
    }
}
