
#include "main.h"

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

void server_listen()
{
    int i=0;
    int num_active_sockets=0;
    char buffer[512];

    if (server_active)
    {
        /* Check for new connections */
		if ((get_player(get_num_players())->socket = SDLNet_TCP_Accept(listen_socket)))
		{
            printf( "New player.\n" );

			if ((get_player(get_num_players())->ip = SDLNet_TCP_GetPeerAddress(get_player(get_num_players())->socket)))
            {
				printf("Player connected: %x %d\n", SDLNet_Read32(&get_player(get_num_players())->ip->host), 
                    SDLNet_Read16(&get_player(get_num_players())->ip->port));
            }
			else
				printf( "SDLNet_TCP_GetPeerAddress: %s\n", SDLNet_GetError());

            /* Add socket to socket set.. */
            SDLNet_TCP_AddSocket(client_sockets, get_player(get_num_players())->socket);

            set_num_players(get_num_players()+1);		
            printf( "We now have %i people connected\n", get_num_players() );
        }

        num_active_sockets=SDLNet_CheckSockets(client_sockets, 0);
        //printf( "Num active: %i\n", num_active_sockets );

        for (i=0;i<get_num_players();i++)
    	{   
            if (SDLNet_SocketReady(get_player(i)->socket))
            {
                //printf( "Player %i active.\n", i );
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
                        case MESG_TILE:
                            //printf( "Tile message\n" );
                            set_tile(get_player_layer(), buffer[1], buffer[2], 1);
                            //printf( "%i,%i\n", buffer[1], buffer[2] );
                            //send_message( buffer, 512 );
                            break;
                    }
        	    }
            }
		}   
    }
}

void start_server( int port )
{
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

    /* Server socket open, and ready to begin listening. Spawn listen thread. */
    server_active=TRUE;
    printf( "Server started.\n" );
}
