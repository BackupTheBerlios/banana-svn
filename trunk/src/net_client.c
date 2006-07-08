
#include "main.h"

/* 
    TODO:
        Ask server to connect. Wait for a an 'okay' before setting client_active.
        Make a 'NEW_PLAYER' message to get from server..
*/

static int client_active;
static int client_slot;

static TCPsocket server_socket;
static IPaddress server_ip;

static char server_hostname[80];
static int server_port;
static char client_nickname[80];

SDLNet_SocketSet server_sockets;

/* v----- TEMPORARY!!--------v*/
void net_change_tile( int x, int y )
{
    char buffer[512];

    buffer[0]=MESG_TILE;
    buffer[1]=x;
    buffer[2]=y;

    send_message( buffer, 3 );
}
/* ^----- TEMPORARY!!! ----^ */

int get_client_slot()
{
    return client_slot;
}

TCPsocket get_server_socket()
{
    return server_socket;
}

int get_client_active()
{
    return client_active;
}

void send_connect_message()
{
    char buffer[512];
    int len=1;
    buffer[0]=MESG_CONNECT;  

    strcpy(buffer+1,client_nickname);
    len+=strlen(client_nickname);      
    
    if (SDLNet_TCP_Send(get_server_socket(), (void *)buffer, 512) < len)
    {
       printf( "SDLNet_TCP_Send: %s\n", SDLNet_GetError());
       exit(EXIT_FAILURE);
    }
}

void send_disconnect_message()
{
    char buffer[512];
    int len=1;
    buffer[0]=MESG_DISCONNECT;    
    
    if (SDLNet_TCP_Send(get_server_socket(), (void *)buffer, 512) < len)
    {
       printf( "SDLNet_TCP_Send: %s\n", SDLNet_GetError());
       exit(EXIT_FAILURE);
    }
}

void client_listen()
{
    int num_active_sockets=0;
    char buffer[512];

    if (client_active)
    {
        num_active_sockets=SDLNet_CheckSockets(server_sockets, 0);
    
        if (SDLNet_SocketReady(server_socket))
        {
      		if (SDLNet_TCP_Recv(server_socket, buffer, 512))
       		{
                switch( buffer[0] )
                {
                    case MESG_WHICHSLOT:
                        client_slot=buffer[1];
                        printf( "Server says slot %i\n", client_slot );
                        break;
                    case MESG_CHAT:
                        add_chat_buffer_line( buffer+1 );
                        break;
                    case MESG_NEWPLAYER:
                        strcpy(get_player(buffer[1])->name,buffer+2);
                        set_player_active(buffer[1],TRUE);
                        printf( "New player:(%i)%s\n", buffer[1], buffer+2 );
                        break;
                    case MESG_PLAYERMOVED:
                        get_player(buffer[1])->xpos=*(int *)(buffer+2);
                        get_player(buffer[1])->ypos=*(int *)(buffer+6);
                        break;
                    case MESG_TILE:
                        set_tile(get_player_layer(), buffer[1], buffer[2], 1);
                        break;
                }
      	    }
        }   
    }
}

void connect_to_server( char *host, int port, char *nickname )
{
    printf( "Connecting to %s on port %i\n", host, port );

    sprintf(server_hostname, "%s", host );
    server_port=port;

	/* Get the IP of the server from the hostname. */
	if (SDLNet_ResolveHost(&server_ip, server_hostname, server_port))
	{
		printf("SDLNet_ResolveHost: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}
    
	/* Open a connection to the server. */
	if (!(server_socket = SDLNet_TCP_Open(&server_ip)))
	{
		printf("SDLNet_TCP_Open: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}

    server_sockets=SDLNet_AllocSocketSet(1);
    if(!server_sockets) 
    {
        printf("SDLNet_AllocSocketSet: %s\n", SDLNet_GetError());
        exit(1); 
    }
    SDLNet_TCP_AddSocket(server_sockets, server_socket);

    strcpy(client_nickname, nickname);

    send_connect_message();

    client_active=TRUE;
    printf( "Connected to server.\n" );
}
