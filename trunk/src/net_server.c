
#include "main.h"

/* 
    loss of connection? time out stuff? uncleanness?    
*/

int server_active=FALSE;

static TCPsocket listen_socket; 
static IPaddress listen_ip;
static int server_port;

SDLNet_SocketSet client_sockets;

TCPsocket get_client_socket( int index )
{
    return get_player(index)->socket;
}

int get_server_active()
{
    return server_active;
}

void send_newplayer_message( int index )
{
    char buffer[512];

    buffer[0]=MESG_NEWPLAYER;
    buffer[1]=index;
    strcpy(buffer+2,get_player(index)->name);
    printf( "New player:(%i)%s\n", buffer[1], buffer+2 );

    send_message( &buffer, strlen(get_player(index)->name)+2 );
}

void server_listen()
{
    int i=0;
    int num_active_sockets=0;
    int new_player_slot=0;

    char buffer[512];

    if (server_active)
    {
        /* Find a free player slot.. */
        for (i=0;i<MAX_PLAYERS;i++)
        {
            if ( !get_player_active(i) )
            {
                new_player_slot=i;
                break;
            }
        }

        /* Check for new connections */
		if ((get_player(new_player_slot)->socket = SDLNet_TCP_Accept(listen_socket)))
		{
            printf( "New player.\n" );

			if ((get_player(new_player_slot)->ip = SDLNet_TCP_GetPeerAddress(get_player(new_player_slot)->socket)))
            {
				printf("Player connected in slot %i: %x %d\n", new_player_slot, SDLNet_Read32(&get_player(new_player_slot)->ip->host), 
                    SDLNet_Read16(&get_player(new_player_slot)->ip->port));
            }
			else
				printf( "SDLNet_TCP_GetPeerAddress: %s\n", SDLNet_GetError());

            /* Add socket to socket set.. */
            SDLNet_TCP_AddSocket(client_sockets, get_player(new_player_slot)->socket);

            /* Raise player count.. */
            set_player_active(i,TRUE);
            set_num_players(get_num_players()+1);	
            printf( "We now have %i people connected\n", get_num_players() );
        }

        num_active_sockets=SDLNet_CheckSockets(client_sockets, 0);
        //printf( "Num active: %i\n", num_active_sockets );

        for (i=0;i<MAX_PLAYERS;i++)
    	{   
            if (get_player_active(i))
            {
                if (SDLNet_SocketReady(get_player(i)->socket))
                {
                    printf( "Player %i active.\n", i );                    
            		if (SDLNet_TCP_Recv(get_player(i)->socket, buffer, 512))
            		{
                        switch( buffer[0] )
                        {
                            case MESG_CHAT:
                                //printf( "Chat message\n" );
                                add_chat_buffer_line( buffer+1 );
                                //memcpy(chat_buffer, buffer+1, 500);
                                //printf( "Chat buffer is now: %s\n", chat_buffer );
                                break;
                            case MESG_CONNECT:
                                printf( "Client %i's name is %s\n", i, buffer+1 );
                                set_player_name(i,buffer+1);
                                /* Send current player list.. */
                                for(i=0;i<MAX_PLAYERS;i++)
                                {
                                    if (get_player_active(i))
                                        send_newplayer_message( i );
                                }
                                break;
                            case MESG_DISCONNECT:
                                SDLNet_TCP_DelSocket(client_sockets, get_player(i)->socket );
                                SDLNet_TCP_Close(get_player(i)->socket);
                                set_player_active(i,FALSE);
                                printf( "Player %i has disconnected\n", i );
                            case MESG_TILE:
                                printf( "Tile message\n" );
                                set_tile(get_player_layer(), buffer[1], buffer[2], 1);
                                //printf( "%i,%i\n", buffer[1], buffer[2] );
                                send_message( buffer, 512 );
                                break;
                        }
            	    }
                }
	   	   }
        }   
    }
}

void start_server( int port, char *nickname )
{
    int i=0;

    server_port=port;

    /* Get local IP/Port for listening */
	if (SDLNet_ResolveHost(&listen_ip, NULL, server_port))
	{
		printf( "SDLNet_ResolveHost: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}

	/* Open a socket for listening */
	if (!(listen_socket=SDLNet_TCP_Open(&listen_ip)))
	{
		printf( "SDLNet_TCP_Open: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}

    client_sockets=SDLNet_AllocSocketSet(16);
    if(!client_sockets) 
    {
        printf("SDLNet_AllocSocketSet: %s\n", SDLNet_GetError());
        exit(1); //most of the time this is a major error, but do what you want.
    }

    /* Set defaults.. */
    for (i=0;i<MAX_PLAYERS;i++)
    {
        set_player_active(i, FALSE);
    }

    /* Set server player.. */
    set_player_active(0,TRUE);
    set_player_name(0, nickname );
    set_num_players(get_num_players()+1);

    /* Server socket open, and ready to begin listening. Spawn listen thread. */
    server_active=TRUE;
    printf( "Server started.\n" );
}
