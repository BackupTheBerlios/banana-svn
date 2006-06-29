
#include "main.h"

void send_message( char *buffer, int len )
{
    int i=0;

    if (get_server_active())
    {
        /* Server running. Send message to all connected clients */
        printf( "Sending server message...\n" );
        for (i=0;i<get_num_players();i++)
        {
            if (SDLNet_TCP_Send(get_client_socket(i), (void *)buffer, 512) < len)
      	    {
      		    printf( "SDLNet_TCP_Send: %s\n", SDLNet_GetError());
       		    exit(EXIT_FAILURE);
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
    }
}
